/*
  LLVM Code Generator.
 */

#include "builtins.h"
#include "code_gen.h"
#include "utils.h"

llvm::Value *numberGenCode(const Object *Obj) {
  return llvm::ConstantInt::get(llvm::getGlobalContext(),
                                llvm::APInt::APInt(64, Obj->Val->SlNum, true));
}

llvm::Value *LLVMCodeGenerator::symbolGenCode(const Object *Obj) {
  assert(Obj->Type == SymbolTy && "symbolGenCode only accepts Symbols");
  std::string name = std::string(Obj->Val->Symbol->Name);
  llvm::Value *Val = this->NamedValues[name];
  if (!Val) {
    this->SemanticErrors.push(new SemanticError(SE_UseBeforeDecl, Obj));
    return nullptr;
  }
  return Val;
}

llvm::Value *LLVMCodeGenerator::genBuiltinCode(const char *SymbolName,
                                               int Nargs, const Object **Args) {
  assert(Nargs == 2);
  llvm::Value *Left = this->genCode(Args[0]);
  llvm::Value *Right = this->genCode(Args[1]);
  if (!Left || !Right)
    return nullptr;
  return Builder.CreateFAdd(Left, Right, "addtmp");
}

llvm::Value *LLVMCodeGenerator::genCode(const Object *Obj) {
  switch (Obj->Type) {
  case SymbolTy:
    return symbolGenCode(Obj);
  case SlNumTy:
    return numberGenCode(Obj);
  }
}

const std::queue<SemanticError*> LLVMCodeGenerator::semanticErrors() {
  return this->SemanticErrors;
}
