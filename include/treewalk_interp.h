// smallisp/treewalk_interp.h - treewalking interpreter -*- mode: C++ -*- //
/*
  Interpreters have to keep track of a few states:
  - Environment
  - Errors
 */

#ifndef _TREEWALK_INTERP_DEF
#define _TREEWALK_INTERP_DEF

#include "builtins.h"
#include "environment.h"
#include "frame.h"
#include "function.h"
#include "list.h"
#include "objects.h"
#include "specialforms.h"

namespace sl {

namespace interp {

class Treewalker {
 public:
  Treewalker(void) : globals_(builtins::environment), frame_(nullptr){};
  // The environment is returned directly and can be manipulated directly.
  Environment &globals(void) { return globals_; };
  // A pointer to current frame and nullptr if there is not frame.
  Frame *frame(void) const { return frame_; }
  // Set the frame of the interpreter to the frame passed in.
  void set_frame(Frame *f) { frame_ = f; };
  // Eval the object passed in.
  const Object *Eval(const Object &);
  // Handle a special form given the form and the kind of form.
  const Object *HandleSpecialForm(const List &sf, specialforms::SFKind sf_kind);
  // The def special form
  const Object *Define(const List &sf);
  // The set! special form
  // set! is defined to change the bindings of variables of the smallest scope.
  // It requires the symbol to have an existing definition.
  const Object *UnsafeSet(const List &sf);
  // This should return a sl::Function
  const Object *Lambda(const List &sf);

  // Lookup a variable in all scopes.
  const Object *Lookup(const Symbol &);
  // Lookup a symbol in the current scope.
  const Object *LocalLookup(const Symbol &sym);
  // Bind symbol to object in current frame. If no frame, bind to global
  // Every scope/frame can only have a single binding
  // This is the most primitive binding operation.
  const Object *Bind(const Symbol &sym, const Object &obj);
  // Create a definition and bind the symbol to the object.
  const Object *MakeDef(const Symbol &sym, const Object &obj);
  const Object *Call(const Function &func, const List &args);

 private:
  Environment globals_;  // Global bindings symbol table
  Frame *frame_;         // Current stack frame
  const List *EvalList(const List &);
};
}  // namespace interp

}  // namespace sl

#endif
