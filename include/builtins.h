// smallisp/builtins.h - Smallisp Builtins -*- mode: C++ -*- //
#ifndef _BUILTINS_DEF
#define _BUILTINS_DEF

#include "environment.h"
#include "function.h"
#include "objects.h"

namespace sl {

namespace builtins {

const Symbol *kAdd(void);
const Symbol *kSub(void);
const Symbol *kMul(void);
const Symbol *kDiv(void);
const Symbol *kCons(void);

}  // namespace builtins
}  // namespace sl

#endif
