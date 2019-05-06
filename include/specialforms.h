// smallisp/specialforms.h - Smallisp specialforms -*- mode: C++ -*- //
#ifndef _SPECIALFORMS_DEF
#define _SPECIALFORMS_DEF

#include "environment.h"

namespace sl {

class Symbol;

namespace specialforms {

enum class SFKind {
  kInvalidSF,
  kDef,
  kUnsafeSet,
  kLambda,
  kIf,
  kFunc,
};

extern const Symbol &kDef;
extern const Symbol &kUnsafeSet;  // For set!
extern const Symbol &kLambda;
extern const Symbol &kIf;
extern const Symbol &kFunc;

// Check if Symbol is a special form
bool IsSpecialForm(const Symbol &);

// GetKind returns the SFKind of a Symbol
// if Symbol is not a special form, return the special
// value kInvalidSF
SFKind GetKind(const Symbol &);

}  // namespace specialforms
}  // namespace sl

#endif
