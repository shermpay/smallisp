#include "builtins.h"

namespace sl {

namespace builtins {

const Object *Add::operator()(const List &args) const {
  // TODO: Type check
  const Int *left = static_cast<const Int *>(args.First());
  const Int *right =
      static_cast<const Int *>(static_cast<const List *>(args.Rest())->First());
  return &(*left + *right);
}

const Object *Sub::operator()(const List &args) const {
  // TODO: Type check
  const Int *left = static_cast<const Int *>(args.First());
  const Int *right =
      static_cast<const Int *>(static_cast<const List *>(args.Rest())->First());
  return &(*left - *right);
}

const Object *Mul::operator()(const List &args) const {
  // TODO: Type check
  const Int *left = static_cast<const Int *>(args.First());
  const Int *right =
      static_cast<const Int *>(static_cast<const List *>(args.Rest())->First());
  return &(*left * *right);
}

const Object *Div::operator()(const List &args) const {
  // TODO: Type check
  const Int *left = static_cast<const Int *>(args.First());
  const Int *right =
      static_cast<const Int *>(static_cast<const List *>(args.Rest())->First());
  return &(*left / *right);
}

const Object *Eq::operator()(const List &args) const {
  // TODO: Type check
  const Object *left = args.First();
  const Object *right = static_cast<const List *>(args.Rest())->First();
  return Bool::Get(left->IsEqual(right));
}

const Object *Ne::operator()(const List &args) const {
  // TODO: Type check
  const Object *left = args.First();
  const Object *right = static_cast<const List *>(args.Rest())->First();
  return Bool::Get(!(left->IsEqual(right)));
}

#define STATIC_INIT_BUILTIN(name_)             \
  const class name_ &name_(void) {             \
    static class name_ *f = new class name_(); \
    return *f;                                 \
  }

STATIC_INIT_BUILTIN(Add);
STATIC_INIT_BUILTIN(Sub);
STATIC_INIT_BUILTIN(Mul);
STATIC_INIT_BUILTIN(Div);
STATIC_INIT_BUILTIN(Eq);
STATIC_INIT_BUILTIN(Ne);

Environment Defns(void) {
  static Environment defns = {
      {Symbol::Get("nil"), NIL},      {Symbol::Get("true"), &TRUE},
      {Symbol::Get("false"), &FALSE}, {Symbol::Get("add"), &Add()},
      {Symbol::Get("sub"), &Sub()},   {Symbol::Get("mul"), &Mul()},
      {Symbol::Get("div"), &Div()},   {Symbol::Get("eq"), &Eq()},
      {Symbol::Get("ne"), &Ne()},
  };
  return defns;
}
}  // namespace builtins
}  // namespace sl
