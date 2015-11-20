// smallisp/interpreter.h - interpreter -*- mode: C++ -*- //
#ifndef _INTERPRETER_DEF
#define _INTERPRETER_DEF
#include "environment.h"
#include "frame.h"
namespace sl {
class Interpreter {
 public:
  virtual ~Interpreter(){};
  // The environment is returned directly and can be manipulated directly.
  virtual Environment &globals(void) = 0;
  // A pointer to current frame and nullptr if there is not frame.
  virtual Frame *frame(void) const = 0;
  virtual void set_frame(Frame *) = 0;
  virtual const Object *Eval(const Object &) = 0;
};
}  // namespace sl
#endif
