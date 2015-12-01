//- smallisp/function.h - Smallisp function definition --*- mode: C++ -*- //

#ifndef _FUNCTION_DEF
#define _FUNCTION_DEF

#include "frame.h"
#include "interpreter.h"
#include "object.h"

namespace sl {
class List;  // Forward declaration

// Callable is an Abstract Base Class for all function types in smallisp.
class Callable : public Object {
 public:
  DEF_TYPE_OBJ("Function")
  virtual ~Callable(void){};
  const Type &GetType(void) const override { return Callable::TypeObj(); };
  bool IsEqual(const Object &o) const override { return this == &o; };
  virtual const Object &operator()(const List &) const = 0;
  // Return the number of parameters
  virtual std::size_t param_count(void) const = 0;
  const Object &Accept(Visitor &v) const override;
};

// A Smallisp Function Object
// Constructing the function creates a new function definition.
// The function cannot be "called", rather it holds an Object that can be
// evaluated by the interpreter.
class Function : public Callable {
 public:
  static const std::size_t kVarArgs;
  static const Function &Val(Interpreter *interp, const std::string &name,
                             const List &params, const Object &body) {
    return *(new Function(interp, name, params, body));
  }
  Function(Interpreter *interp, const std::string &name, const List &params,
           const Object &body);

  Interpreter *interpreter(void) const { return interp_; };
  const std::string name(void) const { return name_; };
  const List &params(void) const { return params_; };
  std::size_t param_count(void) const override { return param_count_; };
  const Object &body(void) const { return body_; };

  std::size_t Hashcode(void) const override {
    return reinterpret_cast<std::size_t>(this);
  };
  const std::string Str(void) const override {
    return "<function: " + name() + ">";
  };

  const Object &operator()(const List &args) const override;

 private:
  Interpreter *interp_;
  const std::string name_;
  const List &params_;
  std::size_t param_count_;
  // An implicit list of expressions to evaluate
  const Object &body_;
};

}  // namespace sl
#endif
