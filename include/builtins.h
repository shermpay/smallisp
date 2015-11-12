// smallisp/builtins.h - Smallisp Builtins -*- mode: C++ -*- //
#ifndef _BUILTINS_DEF
#define _BUILTINS_DEF

#include "environment.h"
#include "function.h"
#include "objects.h"

namespace sl {

namespace builtins {

extern const Symbol *kAdd;
extern const Symbol *kSub;
extern const Symbol *kMul;
extern const Symbol *kDiv;
extern const Symbol *kCons;

}  // namespace builtins
}  // namespace sl

#endif
