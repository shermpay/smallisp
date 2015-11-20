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
class Add : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "add"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override {
    // TODO: Type check
    const Int *left = static_cast<const Int *>(args.First());
    const Int *right = static_cast<const Int *>(
        static_cast<const List *>(args.Rest())->First());
    return &(*left + *right);
  }
};

class Sub : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "sub"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override {
    // TODO: Type check
    const Int *left = static_cast<const Int *>(args.First());
    const Int *right = static_cast<const Int *>(
        static_cast<const List *>(args.Rest())->First());
    return &(*left - *right);
  }
};

class Mul : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "mul"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override {
    // TODO: Type check
    const Int *left = static_cast<const Int *>(args.First());
    const Int *right = static_cast<const Int *>(
        static_cast<const List *>(args.Rest())->First());
    return &(*left * *right);
  }
};

class Div : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "div"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override {
    // TODO: Type check
    const Int *left = static_cast<const Int *>(args.First());
    const Int *right = static_cast<const Int *>(
        static_cast<const List *>(args.Rest())->First());
    return &(*left / *right);
  }
};

class Eq : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "eq"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override {
    // TODO: Type check
    const Object *left = args.First();
    const Object *right = static_cast<const List *>(args.Rest())->First();
    return Bool::Get(left->IsEqual(right));
  }
};

class Ne : public BuiltinFunction {
 public:
  const std::string name(void) const override { return "ne"; }
  std::size_t param_count(void) const override { return 2; };
  const Object *operator()(const List &args) const override {
    // TODO: Type check
    const Object *left = args.First();
    const Object *right = static_cast<const List *>(args.Rest())->First();
    return Bool::Get(!(left->IsEqual(right)));
  }
};


// Initialize builtin environment
Environment Defns(void);
}  // namespace builtins
}  // namespace sl

#endif
