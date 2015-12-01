#include "function.h"

#include "list.h"
#include "visitor.h"

namespace sl {

const Object &Callable::Accept(Visitor &v) const { return v.Visit(*this); }

const std::size_t Function::kVarArgs = -1;

Function::Function(Interpreter *interp, const std::string &name,
                   const List &params, const Object &body)
    : interp_(interp),
      name_(name),
      params_(params),
      param_count_(params.Count()),
      body_(body){};

const Object &Function::operator()(const List &args) const {
  const Object *ret_val = nullptr;
  if (IsType<List>(this->body())) {
    Frame *frame = new Frame{Environment{}, &this->body()};
    const List *rest_params = &this->params();
    // Bind args
    for (const Object &obj : args) {
      const Symbol &curr_param =
          static_cast<const Symbol &>(rest_params->First());
      frame->locals.insert({&curr_param, &obj});
      rest_params = &static_cast<const List &>(rest_params->Rest());
    }
    if (IsNil(frame->body)) {
      assert(false && "FUNCTION BODY CANNOT BE EMPTY");
      ret_val = new Error("Panic!");
    } else {
      interpreter()->set_frame(frame);
      for (const Object &expr : *static_cast<const List *>(frame->body)) {
        ret_val = &(interpreter()->Eval(expr));
      }
      interpreter()->set_frame(nullptr);
    }
  } else {
    assert(false && "Should not be here.");
    ret_val = new Error("Panic: Function body can only be of type list");
  }
  // unwind stack
  // TODO: change this to use static link
  return *ret_val;
}

}  // namespace sl
