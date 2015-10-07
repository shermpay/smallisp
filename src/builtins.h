/* -*- mode: C++ -*- */
#ifndef _BUILTINS_DEF
#define _BUILTINS_DEF

#include "sltypes.h"

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <unordered_map>
#include <unordered_set>
#include <string>

namespace builtins {

enum SpecialFormType {
  SFT_If,
  SFT_Def,
};

extern const Symbol *DefSymbol;
extern const Symbol *DefFnSymbol;
extern const Symbol *IfSymbol;

extern const std::unordered_map<const Symbol *, SlMacro *> SpecialForms;

extern const Symbol *AddSymbol;
extern const Symbol *SubSymbol;
extern const Symbol *MulSymbol;

extern const std::unordered_map<const Symbol *, SlFunction *> Defs;

} // namespace Builtins

#endif
