// smallisp/builtins.h - Smallisp Builtins -*- mode: C++ -*- //
#ifndef _BUILTINS_DEF
#define _BUILTINS_DEF

#include "environment.h"
#include "function.h"
#include "objects.h"

namespace sl {

namespace builtins {

const Symbol *NilSym(void);
const Symbol *TrueSym(void);
const Symbol *FalseSym(void);

const Symbol *AddSym(void);
const Symbol *SubSym(void);
const Symbol *MulSym(void);
const Symbol *DivSym(void);
const Symbol *ConsSym(void);

}  // namespace builtins
}  // namespace sl

#endif
