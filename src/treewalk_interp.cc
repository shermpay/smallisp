#include "treewalk_interp.h"

#include <functional>
#include <list>
#include <utility>

#include "builtins.h"
#include "function.h"
#include "list.h"
#include "objects.h"
#include "specialforms.h"

namespace sl {

namespace interp {

const Object *Treewalker::Eval(const Object &obj) { return obj.Accept(*this); }

const Object *Treewalker::HandleSpecialForm(const List &sf,
                                            specialforms::SFKind sf_kind) {
  using namespace specialforms;
  const Object *ret = nullptr;
  switch (sf_kind) {
    case SFKind::kInvalidSF:
      assert(false && "Special Form Invalid cannot be handled.");
      break;
    case SFKind::kDef:
      ret = Def(sf);
      break;
    case SFKind::kUnsafeSet: {
      ret = UnsafeSet(sf);
      break;
    }
    case SFKind::kLambda: {
      ret = Lambda(sf);
      break;
    }
    case SFKind::kFunc: {
      ret = Func(sf);
      break;
    }
    case SFKind::kIf: {
      ret = If(sf);
      break;
    }
    default: {
      assert(false && "Special Form not handled, this is a programming error!");
    }
  }
  return ret;
}

const Object *Treewalker::Def(const List &sf) {
  // Guaranteed to have at least 1 element
  const List &define_expr = *static_cast<const List *>(sf.Rest());
  if (define_expr.Count() != 2)
    return new Error(
        "syntax error: define expression should be of the form "
        "(define symbol expr)");
  if (IsType<Symbol>(define_expr.First())) {
    const Symbol &define_sym =
        *static_cast<const Symbol *>(define_expr.First());
    const List &define_body = *static_cast<const List *>(define_expr.Rest());
    const Object &obj = *Eval(*define_body.First());
    return MakeDef(define_sym, obj);
  } else {
    // Error: cannot bind to non-symbol
    return new Error("syntax error: def cannot bind to non-symbol");
  }
}

const Object *Treewalker::UnsafeSet(const List &sf) {
  const List &set_expr = *static_cast<const List *>(sf.Rest());
  if (set_expr.Count() != 2)
    return new Error(
        "syntax error: set! expression should be of the form "
        "(set! symbol expr)");
  if (IsType<Symbol>(set_expr.First())) {
    const Symbol &set_sym = *static_cast<const Symbol *>(set_expr.First());
    if (*Lookup(set_sym) != Error("")) {
      const List &set_body = *static_cast<const List *>(set_expr.Rest());
      const Object &obj = *Eval(*set_body.First());
      return Bind(set_sym, obj);
    } else {
      return new Error("Cannot set! an undefined symbol");
    }
  } else {
    // Error: cannot bind to non-symbol
    return new Error("set! invalid: cannot bind to non-symbol");
  }
}

static const Object *MakeFunction(Interpreter *interp, const std::string &name,
                                  const sl::List &form) {
  if (IsType<List>(form.First())) {
    const List &param_list = *static_cast<const List *>(form.First());
    // TODO: check this
    // Environment &curr_env = frame() ? frame()->locals : globals();
    const List *body = static_cast<const List *>(form.Rest());
    return new Function(interp, name, param_list, *body);
  } else {
    return new Error("function expression invalid: expects param list");
  }
}

const Object *Treewalker::Lambda(const sl::List &sf) {
  const List &lambda_expr = *static_cast<const List *>(sf.Rest());
  if (lambda_expr.Count() < 2)
    return new Error(
        "syntax error: lambda expression should be of the form "
        "(lambda (params) body)");
  return MakeFunction(this, "<lambda>", lambda_expr);
}

const Object *Treewalker::Func(const sl::List &sf) {
  const List &func_expr = *static_cast<const List *>(sf.Rest());
  if (func_expr.Count() < 3)
    return new Error(
        "syntax error: func statement should be of the form "
        "(func name (params) body)");
  if (IsType<Symbol>(func_expr.First())) {
    const Symbol &func_sym = *static_cast<const Symbol *>(func_expr.First());
    const List &func_body = *static_cast<const List *>(func_expr.Rest());
    const Object *func = MakeFunction(this, func_sym.name(), func_body);
    return MakeDef(func_sym, *func);
  }
  return new Error(
      "syntax error: func declaration missing name"
      "\nexpects (func name (param) body)");
}

const Object *Treewalker::If(const List &sf) {
  const List &if_expr = *static_cast<const List *>(sf.Rest());
  if (if_expr.Count() != 3)
    return new Error(
        "syntax error: if expression should be of the form "
        "(if cond then else)");
  const Object &if_cond = *Eval(*if_expr.First());
  if (IsType<Bool>(if_cond)) {
    // Check Syntax
    const List &if_body = *static_cast<const List *>(if_expr.Rest());
    assert(IsType<List>(if_body) && "if body should be list");
    if (bool(static_cast<const Bool &>(if_cond))) {
      return Eval(*if_body.First());
    } else {
      return Eval(*static_cast<const List *>(if_body.Rest())->First());
    }
  } else {
    return new Error(
        "if condition must be of type Bool"
        "\ncondition: " +
        if_cond.Str());
  }
}

const Object *Treewalker::LocalLookup(const Symbol &sym) {
  Environment local_env = frame() ? frame()->locals : globals();
  auto iter = local_env.find(&sym);
  if (iter != local_env.end()) return iter->second;
  return kVoid;  // Nothing found
}

// Lookup bindings in all scopes
// This is invalid. This needs something that traces static links
// TODO: change this when static links are implemented
const Object *Treewalker::Lookup(const Symbol &sym) {
  const Object *obj = LocalLookup(sym);
  if (obj == kVoid) {
    auto iter = globals().find(&sym);
    if (iter != globals().end()) {
      obj = iter->second;
    } else {
      return new Error("Symbol '" + sym.Str() + "' unbound.");
    }
  }
  return obj;
};

const Object *Treewalker::Bind(const Symbol &sym, const Object &obj) {
  Environment &curr_env = frame() ? frame()->locals : globals();
  curr_env[&sym] = &obj;
  return kVoid;
}

const Object *Treewalker::MakeDef(const Symbol &sym, const Object &obj) {
  if (LocalLookup(sym) != kVoid)
    return new Error("Cannot redefine '" + sym.Str() + "' in same scope.");
  return Bind(sym, obj);
}

const Object *Treewalker::Call(const Callable &func, const List &args) {
  std::size_t param_count = func.param_count();
  std::size_t arg_count = args.Count();
  if (param_count != arg_count) {
    return new Error("Calling " + func.Str() + " with invalid number of args." +
                     "\n  expected: " + std::to_string(param_count) +
                     "\n  actual:   " + std::to_string(arg_count));
  }

  return func(args);
}
////////////////////////
// Implements Visitor //
////////////////////////
const Object *Treewalker::Visit(const Byte &o) { return &o; }
const Object *Treewalker::Visit(const Int &o) { return &o; }
const Object *Treewalker::Visit(const Bool &o) { return &o; }
const Object *Treewalker::Visit(const Symbol &obj) {
  const Symbol *sym = static_cast<const Symbol *>(&obj);
  if (specialforms::IsSpecialForm(*sym)) {
    return new Error("Cannot evaluate special form: " + sym->Str());
  }
  return Lookup(*sym);
}
const Object *Treewalker::Visit(const Void &) {
  assert(false && "Error");
  return kVoid;
}
const Object *Treewalker::Visit(const Error &o) { return &o; }

const Object *Treewalker::Visit(const ConsC &) {
  assert(false && "NOT IMPLEMENTED");
  return kVoid;
}

const Object *Treewalker::Visit(const List &obj) {
  // TODO: Change this once we have Sexprs.
  // For now we do not have quoted lists, so a list is a S-Expression.
  if (IsNil(&obj)) return new Error("Cannot evaluate nil");

  const List &lst = static_cast<const List &>(obj);
  const Object *head = lst.First();
  if (IsType<Symbol>(head)) {
    const Symbol &head_sym = *static_cast<const Symbol *>(head);
    specialforms::SFKind sf_kind = specialforms::GetKind(head_sym);
    if (sf_kind != specialforms::SFKind::kInvalidSF) {
      return HandleSpecialForm(lst, sf_kind);
    }
  }
  const List &func_lst = *EvalList(lst);
  if (IsType<Function>(func_lst.First())) {
    return Call(*static_cast<const Function *>(func_lst.First()),
                *static_cast<const List *>(func_lst.Rest()));
  } else {
    return new Error("Cannot call non-function in expression " +
                     func_lst.Str());
  }
}
const Object *Treewalker::Visit(const Nil &o) {
  assert(false && "Nil Dereference");
  return &o;
}

const Object *Treewalker::Visit(const Callable &o) {
  assert(false && "NOT IMPLEMENTED");
  return &o;
}

void Treewalker::Print(void) const {
  std::cout << "Globals: {\n";
  for (const auto pair : globals_) {
    assert(pair.first != nullptr && pair.second != nullptr);
    std::cout << pair.first->Str() << " => " << pair.second->Str() << "\n";
  }
  std::cout << "}" << std::endl;
}

const List *Treewalker::EvalList(const List &lst) {
  if (IsNil(&lst)) {
    return NIL;
  } else {
    return Cons(Eval(*lst.First()),
                EvalList(*static_cast<const List *>(lst.Rest())));
  }
}

}  // namespace interp

}  // namespace sl
