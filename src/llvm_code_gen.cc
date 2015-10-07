/*
  LLVM Code Generator.
 */

#include "builtins.h"
#include "code_gen.h"
#include "utils.h"
#include "sltypes.h"
#include "list.h"

#include <cstdio>

#include <vector>

llvm::Value *numberGenCode(const Object *Obj) {
  return llvm::ConstantInt::get(llvm::getGlobalContext(),
                                llvm::APInt::APInt(64, Obj->Val->SlNum, true));
}

llvm::Value *LLVMCodeGenerator::symbolGenCode(const Object *Obj) {
  assert(Obj->Type == SymbolTy && "symbolGenCode only accepts Symbols");
  std::string name = std::string(Obj->Val->Symbol->name());
  llvm::Value *Val = this->NamedValues[name];
  if (!Val) {
    this->SemanticErrors.push(new SemanticError(SE_UseBeforeDecl, Obj));
    return nullptr;
  }
  return Val;
}

llvm::Value *LLVMCodeGenerator::callGenCode(const Object *Obj) {
  assert(Obj->Type == ListTy && "Call should take a list type");
  List *Lst = Obj->Val->List;
  assert(Lst->Head->Val->Type == SymbolTy &&
         "Call should have a Symbol as Head");
  const Symbol *FuncSymbolPtr(Lst->Head->Val->Val->Symbol);

  if (builtins::Defs.find(FuncSymbolPtr) != builtins::Defs.end()) {
    return builtinGenCode(*FuncSymbolPtr, listTail(Lst));
  } else {
    return nullptr;
  };
}

llvm::Value *LLVMCodeGenerator::builtinGenCode(const Symbol &FuncSymbol,
                                               const List *Args) {
  std::vector<llvm::Value *> Values;
  for (Cons *Curr = Args->Head; Curr; Curr = Curr->Next) {
    llvm::Value *Val = this->genCode(Curr->Val);
    Values.push_back(Val);
    if (Values.size() > 2) {
      return nullptr;
    }
  }
  if (!Values[0] || !Values[1])
    return nullptr;
  if (FuncSymbol.name() == "add")
    return this->builder().CreateNSWAdd(Values[0], Values[1], "addtmp");
  else if (FuncSymbol.name() == "sub")
    return this->builder().CreateNSWSub(Values[0], Values[1], "subtmp");
  else if (FuncSymbol.name() == "mul")
    return this->builder().CreateNSWMul(Values[0], Values[1], "subtmp");
  else
    return nullptr;
}

llvm::Value *LLVMCodeGenerator::genCode(const Object *Obj) {
  switch (Obj->Type) {
  case SymbolTy:
    return symbolGenCode(Obj);
  case SlNumTy:
    return numberGenCode(Obj);
  case SexpTy:
    return callGenCode(Obj);
  default:
    fprintf(stderr, "Error Type: %d\n", Obj->Type);
    return nullptr;
  }
}

const std::queue<SemanticError *> LLVMCodeGenerator::semanticErrors() {
  return this->SemanticErrors;
}
