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
#include "list.h"
#include "objects.h"
#include "specialforms.h"

namespace sl {

namespace interp {

struct Error {
  const std::string msg;
};

class Treewalker {
 public:
  Treewalker(void)
      : globals_(builtins::environment), frame_(nullptr), error_(nullptr){};
  // The environment is returned directly and can be manipulated directly.
  Environment &globals(void) { return globals_; };
  Frame *frame(void) const { return frame_; }
  void set_frame(Frame *f) { frame_ = f; };
  const Error error(void) const { return *error_; };
  const Object *Eval(const Object &);
  const Object *Lookup(const Symbol &);
  void Bind(const Symbol &sym, const Object &obj);
  void Failed(const std::string &msg) { error_ = new struct Error({msg}); };
  void HandleErrors(void);
  const Object *SpecialFormHandler(const List &sf,
                                   specialforms::SFKind sf_kind);
  void UnsafeSet(const List &sf);

 private:
  Environment globals_;  // Global bindings symbol table
  Frame *frame_;         // Current stack frame

  Error *error_;  // Error
};

}  // namespace interp

}  // namespace sl

#endif
