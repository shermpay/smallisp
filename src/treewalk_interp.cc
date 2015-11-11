#include "treewalk_interp.h"

#include "specialforms.h"
#include "list.h"
#include "objects.h"
#include <vector>

namespace sl {

namespace interp {
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
      std::vector<const Object *> vec;
      for (const auto &elt : lst) {
        vec.push_back(Eval(elt));
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
    case SFKind::kDefine:
      ret = Define(sf);
      break;
    case SFKind::kUnsafeSet: {
      ret = UnsafeSet(sf);
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

}  // namespace interp

}  // namespace sl
