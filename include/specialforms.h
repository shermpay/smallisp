// smallisp/specialforms.h - Smallisp specialforms -*- mode: C++ -*- //
#ifndef _SPECIALFORMS_DEF
#define _SPECIALFORMS_DEF

#include "environment.h"
#include "objects.h"

namespace sl {

namespace specialforms {

enum class SFKind {
  kInvalidSF,
  kDefine,
  kUnsafeSet,
};

extern const Symbol &kDefine;
extern const Symbol &kUnsafeSet;

// Check if Symbol is a special form
bool IsSpecialForm(const Symbol &);

// GetKind returns the SFKind of a Symbol
// if Symbol is not a special form, return the special
// value kInvalidSF
SFKind GetKind(const Symbol &);

}  // namespace specialforms
}  // namespace sl

#endif
