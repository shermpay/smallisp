#include "treewalk_interp.h"

#include <list>

#include "builtins.h"
#include "function.h"
#include "list.h"
#include "objects.h"
#include "specialforms.h"

namespace sl {

namespace interp {

namespace treewalker {
class BuiltinAdd : public CodeObject {
 public:
  const Object *operator()(const List &args) const override {
    if (args.First()->GetType() == Type::kInt &&
        args.Rest()->First()->GetType() == Type::kInt) {
      const Int &left = *static_cast<const Int *>(args.First());
      const Int &right = *static_cast<const Int *>(args.Rest()->First());
      return &(left + right);
    } else
      return new Error("binary add requires 2 ints");
  }
  const std::string Str(void) const override { return "add"; }
  static const BuiltinAdd &kInstance;
};
const BuiltinAdd &kBuiltinAdd = *(new BuiltinAdd());

const Function *MakeFunction(const List *params, const CodeObject &co) {
  return new Function(Environment{}, *params, co);
}

}  // namespace treewalker

Environment Treewalker::builtins = {
    {Symbol::Get("add"),
     treewalker::MakeFunction(new List({Symbol::Get("x"), Symbol::Get("y")}),
                              treewalker::kBuiltinAdd)},
};

const Object *Treewalker::Eval(const Object &obj) {
  const Object *result = nullptr;
  switch (obj.GetType()) {
    case Type::kInt: {
      return &obj;
    }
    case Type::kSymbol: {
      const Symbol *sym = static_cast<const Symbol *>(&obj);
      if (specialforms::IsSpecialForm(*sym)) {
        result = new Error("Cannot evaluate special form: " + sym->Str());
      }
      result = Lookup(*sym);
      break;
    }
    case Type::kList: {
      // TODO: Change this once we have Sexprs.
      // For now we do not have quoted lists, so a list is a S-Expression.
      const List &lst = static_cast<const List &>(obj);
      const Object *head = lst.First();
      if (head->GetType() == Type::kSymbol) {
        const Symbol &head_sym = *static_cast<const Symbol *>(head);
        specialforms::SFKind sf_kind = specialforms::GetKind(head_sym);
        if (sf_kind != specialforms::SFKind::kInvalidSF) {
          return HandleSpecialForm(lst, sf_kind);
        }
      }
      const List &func_lst = *EvalList(lst);
      if (func_lst.First()->GetType() == Type::kFunction) {
        result = Call(*static_cast<const Function *>(func_lst.First()),
                      *func_lst.Rest());
      } else {
        result = new Error("Cannot call non-function in expression " +
                           func_lst.Str());
      }

      break;
    }
    default: {
      // Error
      result = new Error("Unable to recognize type of Object!");
    }
  }
  return result;
}

const Object *Treewalker::HandleSpecialForm(const List &sf,
                                            specialforms::SFKind sf_kind) {
  using namespace specialforms;
  const Object *ret = nullptr;
  switch (sf_kind) {
    case SFKind::kInvalidSF:
      assert(false && "Special Form Invalid cannot be handled.");
      break;
    case SFKind::kDef:
      ret = Define(sf);
      break;
    case SFKind::kUnsafeSet: {
      ret = UnsafeSet(sf);
      break;
    }
    case SFKind::kLambda: {
      ret = Lambda(sf);
      break;
    }
    default: {
      assert(false && "Special Form not handled, this is a programming error!");
    }
  }
  return ret;
}

const Object *Treewalker::Define(const List &sf) {
  const List &define_expr = *sf.Rest();
  if (define_expr.First()->GetType() == Type::kSymbol) {
    const Symbol &define_sym =
        *static_cast<const Symbol *>(define_expr.First());
    const List &define_body = *define_expr.Rest();
    if (define_body.Count() == 1) {
      const Object &obj = *Eval(*define_body.First());
      return MakeDef(define_sym, obj);
    } else {
      // Error: (define symbol expr)
      return new Error("define syntax invalid: expects (define! symbol expr)");
    }
  } else {
    // Error: cannot bind to non-symbol
    return new Error("define invalid: cannot bind to non-symbol");
  }
}

const Object *Treewalker::UnsafeSet(const List &sf) {
  const List &set_expr = *sf.Rest();
  if (set_expr.First()->GetType() == Type::kSymbol) {
    const Symbol &set_sym = *static_cast<const Symbol *>(set_expr.First());
    if (*Lookup(set_sym) != Error("")) {
      const List &set_body = *set_expr.Rest();
      if (set_body.Count() == 1) {
        const Object &obj = *Eval(*set_body.First());
        return Bind(set_sym, obj);
      } else {
        // Error: (set! symbol expr)
        return new Error("set! syntax invalid: expects (set! symbol expr)");
      }
    } else {
      return new Error("Cannot set! an undefined symbol");
    }
  } else {
    // Error: cannot bind to non-symbol
    return new Error("set! invalid: cannot bind to non-symbol");
  }
}

const Object *Treewalker::Lambda(const sl::List &sf) {
  const List &lambda_expr = *sf.Rest();
  if (lambda_expr.First()->GetType() == Type::kList) {
    const List &param_list = *static_cast<const List *>(lambda_expr.First());
    Environment &curr_env = frame() ? frame()->locals : globals();
    return new Function(curr_env, param_list, *lambda_expr.Rest());
  } else {
    return new Error("lambda invalid: expects param list");
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
  if (obj == kVoid) return new Error("Symbol '" + sym.Str() + "' unbound.");
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

const Object *Treewalker::Call(const Function &func, const List &args) {
  const Object *ret_val;
  if (func.body().GetType() == Type::kList) {
    Frame *frame = new Frame{Environment{}, &func.body()};
    const List &func_params = func.params();
    const ConsC *curr_param = func_params.head();
    // Bind args
    for (const Object &obj : args) {
      const Symbol &param = *static_cast<const Symbol *>(curr_param->car());
      frame->locals.insert({&param, &obj});
    }
    this->set_frame(frame);
    for (const Object &expr : *static_cast<const List *>(frame->body)) {
      ret_val = Eval(expr);
    }
    this->set_frame(nullptr);
  } else if (func.body().GetType() == Type::kCodeObject) {
    // Body is a builtin object
    const treewalker::CodeObject &builtin_fn =
        static_cast<const treewalker::CodeObject &>(func.body());
    ret_val = builtin_fn(args);
  } else {
    assert(false && "should not be here");
    ret_val = new Error("");
  }
  // unwind stack
  // TODO: change this to use static link
  return ret_val;
}

void Treewalker::Print(void) const {
  std::cout << "Globals: {\n";
  for (const auto pair : globals_) {
    std::cout << pair.first << " => " << pair.second->Str() << "\n";
  }
  std::cout << "}" << std::endl;
}

const List *Treewalker::EvalList(const List &lst) {
  if (IsNil(&lst)) {
    return kNil();
  } else {
    return Cons(Eval(*lst.First()), EvalList(*lst.Rest()));
  }
}
}  // namespace interp

}  // namespace sl
