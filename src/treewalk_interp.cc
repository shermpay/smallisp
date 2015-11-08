#include "treewalk_interp.h"

#include "specialforms.h"
#include "list.h"
#include "objects.h"
#include <vector>

namespace sl {

namespace interp {
const Object *Treewalker::Eval(const Object &obj) {
  std::cout << "Encountered Type -> " << obj.GetType() << std::endl;
  const Object *result = nullptr;
  switch (obj.GetType()) {
    case Type::kInt: {
      return &obj;
    }
    case Type::kSymbol: {
      const Symbol *sym = static_cast<const Symbol *>(&obj);
      if (specialforms::IsSpecialForm(*sym)) {
        Failed("Cannot evaluate special form: " + sym->Str());
        break;
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
          return SpecialFormHandler(lst, sf_kind);
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
      Failed("Unable to recognize type of Object!");
    }
  }
  HandleErrors();
  return result;
}

// Lookup bindings
const Object *Treewalker::Lookup(const Symbol &sym) {
  if (frame() && ((frame()->locals.find(&sym)) == frame()->locals.end())) {
    // inside frame
    auto iter = frame()->locals.find(&sym);
    return iter->second;
  } else {
    // in global space
    auto iter = globals().find(&sym);
    if (iter == globals().end()) {
      Failed("Symbol '" + sym.Str() + "' does not exist!");
      return nullptr;
    } else {
      return iter->second;
    }
  }
};

// Bind symbol to object in current frame. If no frame, bind to global
void Treewalker::Bind(const Symbol &sym, const Object &obj) {
  if (frame()) {
    // inside frame
    frame()->locals.insert({&sym, &obj});
  } else {
    // bind to globals
    globals().insert({&sym, &obj});
  }
}

// Output errors to stderr and cleanup
void Treewalker::HandleErrors(void) {
  if (error_ != nullptr) {
    fprintf(stderr, "%s\n", error().msg.c_str());
    error_ = nullptr;
  }
};

const Object *Treewalker::SpecialFormHandler(const List &sf,
                                             specialforms::SFKind sf_kind) {
  using namespace specialforms;
  switch (sf_kind) {
    case SFKind::kInvalidSF:
      assert(false && "Special Form Invalid cannot be handled.");
      return nullptr;
    case SFKind::kUnsafeSet: {
      UnsafeSet(sf);
      return kNil;
    }
  }
}

void Treewalker::UnsafeSet(const List &sf) {
  assert(sf.First()->GetType() == Type::kSymbol);
  const List &set_expr = *sf.Rest();
  if (set_expr.First()->GetType() == Type::kSymbol) {
    const Symbol &set_sym = *static_cast<const Symbol *>(set_expr.First());
    const List &set_body = *set_expr.Rest();
    if (set_body.Count() == 1) {
      const Object &obj = *Eval(*set_body.First());
      Bind(set_sym, obj);
    } else {
      // Error: (set! symbol expr)
      Failed("set! syntax invalid: expects (set! symbol expr)");
    }
  } else {
    // Error: cannot bind to non-symbol
    Failed("set! invalid: cannot bind to non-symbol");
  }
}

}  // namespace interp

}  // namespace sl
