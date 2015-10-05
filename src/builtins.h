/* -*- mode: C++ -*- */
#ifndef _BUILTINS_DEF
#define _BUILTINS_DEF

#include "code_gen.h"

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <map>
#include <string>

enum SpecialFormType {
  SFT_If,
  SFT_Def,
};

class SpecialForm {
public:
  virtual llvm::Value *genCode(int Nargs, Object** Args) = 0;
  virtual  ~SpecialForm();
};

const char* BuiltinNames[] = {"add"};

#endif
