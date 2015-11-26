#include "builtins.h"

namespace sl {

namespace builtins {

static inline const Error *TypeCheck(const BuiltinFunction *func,
                                     const Object *obj, Type &expected) {
  if (obj->GetType() != expected)
    return new Error("Error in " + func->Str() +
                     "\n   error: type mismatch\n  expect: " + expected.Str() +
                     "\n  actual: " + obj->GetType().Str());
  return nullptr;
}

const Object *Add::operator()(const List &args) const {
  // TODO: Type check
  assert(!IsNil(&args) && "param/arg counting happens prior");
  const Error *err;
  if ((err = TypeCheck(this, args.First(), Int::TypeObj()))) return err;
  const Int *left = static_cast<const Int *>(args.First());
  const List &rest = *static_cast<const List *>(args.Rest());
  if ((err = TypeCheck(this, rest.First(), Int::TypeObj()))) return err;
  const Int *right = static_cast<const Int *>(rest.First());
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

const Object *Cons::operator()(const List &args) const {
  const Object *left = args.First();
  const Object *right = static_cast<const List *>(args.Rest())->First();
  if (IsType<List>(right)) {
    return sl::Cons(left, static_cast<const List *>(right));
  } else {
    return sl::Cons(left, right);
  }
}

const Object *Car::operator()(const List &args) const {
  const Object *obj = args.First();
  if (IsType<List>(obj)) {
    return static_cast<const List *>(obj)->First();
  } else if (IsType<ConsC>(obj)) {
    return static_cast<const ConsC *>(obj)->car();
  } else {
    return new Error("Error: 'car' can only be applied to cons or list");
  }
}

const Object *Cdr::operator()(const List &args) const {
  const Object *obj = args.First();
  if (IsType<List>(obj)) {
    return static_cast<const List *>(obj)->Rest();
  } else if (IsType<ConsC>(obj)) {
    return static_cast<const ConsC *>(obj)->cdr();
  } else {
    return new Error("Error: 'car' can only be applied to cons or list");
  }
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
STATIC_INIT_BUILTIN(Cons);
STATIC_INIT_BUILTIN(Car);
STATIC_INIT_BUILTIN(Cdr);

Environment Defns(void) {
  static Environment defns = {
      {Symbol::Get("nil"), NIL},      {Symbol::Get("true"), &TRUE},
      {Symbol::Get("false"), &FALSE}, {Symbol::Get("add"), &Add()},
      {Symbol::Get("sub"), &Sub()},   {Symbol::Get("mul"), &Mul()},
      {Symbol::Get("div"), &Div()},   {Symbol::Get("eq"), &Eq()},
      {Symbol::Get("ne"), &Ne()},     {Symbol::Get("cons"), &Cons()},
      {Symbol::Get("car"), &Car()},   {Symbol::Get("cdr"), &Cdr()},
  };
  return defns;
}
}  // namespace builtins
}  // namespace sl
