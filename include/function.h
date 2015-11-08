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
  Function(const Environment env, const List *params, const Object *body)
      : params_(params), body_(body), env_(env){};
  virtual ~Function() {
    delete params_;
    delete body_;
  };

  const List *params(void) const { return params_; };
  const Object *body(void) const { return body_; };
  virtual Type GetType(void) const override { return Type::kFunction; };
  virtual bool IsEqual(const Object *o) const override { return this == o; };
  virtual bool IsEqual(const Object &o) const override {
    return this->IsEqual(&o);
  };
  // Creates a Smallisp Stack Frame
  const Frame *MakeFrame(const List *args) const;

 private:
  const List *params_;
  const Object *body_;
  // Make this function a *closure*
  const Environment env_;
};

}  // namespace sl
#endif
