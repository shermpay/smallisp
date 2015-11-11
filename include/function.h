//- smallisp/function.h - Smallisp function definition --*- mode: C++ -*- //

#ifndef _FUNCTION_DEF
#define _FUNCTION_DEF

#include "environment.h"
#include "frame.h"
#include "list.h"
#include "objects.h"

namespace sl {

// A Smallisp Function Object
// Constructing the function creates a new function definition.
// The function cannot be "called", rather it is able to create stack frames
// which can be given to an interpreter to be executed.
class Function : public Object {
 public:
  // Environment is enclosing environment
  Function(const Environment env, const List &params, const List &body)
      : params_(params), body_(body), env_(env){};

  const List &params(void) const { return params_; };
  const List &body(void) const { return body_; };
  Type GetType(void) const override { return Type::kFunction; };
  bool IsEqual(const Object *o) const override { return this == o; };
  bool IsEqual(const Object &o) const override { return this->IsEqual(&o); };

  std::size_t Hashcode(void) const override {
    return reinterpret_cast<std::size_t>(this);
  };
  const std::string Str(void) const override {
    return "Function: " + params().Str() + " -> Object";
  };

 private:
  const List &params_;
  // An implicit list of expressions to evaluate
  const List &body_;
  // Make this function a *closure*
  const Environment env_;
};

}  // namespace sl
#endif
