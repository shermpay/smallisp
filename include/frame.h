//- smallisp/frame.h - Smallisp frame definition --*- mode: C++ -*- //
#ifndef _FRAME_DEF
#define _FRAME_DEF

#include "environment.h"
#include "list.h"

namespace sl {

// Smallisp Frame Structure
struct Frame {
  Environment locals;  // Local bindings symbol table
  const Object *body;
  /* Missing Fields
     static_link: Link to previous frame
   */
};
}
#endif
