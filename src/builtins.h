/* -*- mode: C++ -*- */
#ifndef _BUILTINS_DEF
#define _BUILTINS_DEF

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <unordered_map>
#include <string>

enum SpecialFormType {
  SFT_If,
  SFT_Def,
};

class BuiltinDef {
public:
  int NumArgs;

  BuiltinDef(int NumArgs) { this->NumArgs = NumArgs; }
};

const std::unordered_map<std::string, BuiltinDef *> BuiltinDefs = {
    {"add", new BuiltinDef(2)},
    {"sub", new BuiltinDef(2)},
    {"mul", new BuiltinDef(2)},
};

#endif
