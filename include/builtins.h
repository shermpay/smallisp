// smallisp/builtins.h - Smallisp Builtins -*- mode: C++ -*- //
#ifndef _BUILTINS_DEF
#define _BUILTINS_DEF

#include <functional>
#include "environment.h"
#include "function.h"
#include "list.h"
#include "objects.h"

namespace sl {

namespace builtins {

// Builtin Function is the Abstract Base Class for all builtin functions
class BuiltinFunction : public Callable {
 public:
  virtual const std::string name(void) const = 0;

  std::size_t Hashcode(void) const override {
    return reinterpret_cast<std::size_t>(this);
  };
  const std::string Str(void) const override {
    return "<builtin function: " + name() + ">";
  };

  virtual std::size_t param_count(void) const override = 0;
  virtual const Object *operator()(const List &args) const override = 0;
};

// TODO: Implement builtin functions with templates
// TODO: Or implement them to hold pointers to native functions
class Add : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "add"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override;
};

class Sub : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "sub"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override;
};

class Mul : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "mul"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override;
};

class Div : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "div"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override;
};

class Eq : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "eq"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override;
};

class Ne : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "ne"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override;
};

class Cons : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "cons"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override;
};

class Car : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "car"; }
  std::size_t param_count(void) const override { return 1; };
  const Object *operator()(const List &args) const override;
};

class Cdr : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "cdr"; }
  std::size_t param_count(void) const override { return 1; };
  const Object *operator()(const List &args) const override;
};

const class Add &Add(void);
const class Sub &Sub(void);
const class Mul &Mul(void);
const class Div &Div(void);
const class Eq &Eq(void);
const class Ne &Ne(void);
const class Cons &Cons(void);
const class Car &Car(void);
const class Cdr &Cdr(void);

// Initialize builtin environment
Environment Defns(void);
}  // namespace builtins
}  // namespace sl

#endif
