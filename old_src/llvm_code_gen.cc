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
  const Symbol *SymbolPtr(Lst->Head->Obj->Val->Symbol);

  if (builtins::Defs.find(SymbolPtr) != builtins::Defs.end()) {
    return builtinGenCode(*SymbolPtr, listTail(Lst));
  } else if (builtins::SpecialForms.find(SymbolPtr) !=
             builtins::SpecialForms.end()) {
    return specialFormGenCode(*SymbolPtr, listTail(Lst));
  } else {
    return fnCallGenCode(*SymbolPtr, listTail(Lst));
  };
}

llvm::Value *LLVMCodeGenerator::builtinGenCode(const Symbol &FuncSymbol,
                                               const List *Args) {
  std::vector<llvm::Value *> Values;
  for (Cons *Curr = Args->Head; Curr; Curr = Curr->Next) {
    llvm::Value *Val = this->genCode(Curr->Obj);
    Values.push_back(Val);
    if (Values.size() > 2) {
      return nullptr;
    }
  }
  if (!Values[0] || !Values[1])
    return nullptr;
  if (FuncSymbol.name() == "add")
    return this->Builder.CreateNSWAdd(Values[0], Values[1], "addtmp");
  else if (FuncSymbol.name() == "sub")
    return this->Builder.CreateNSWSub(Values[0], Values[1], "subtmp");
  else if (FuncSymbol.name() == "mul")
    return this->Builder.CreateNSWMul(Values[0], Values[1], "multmp");
  else
    return nullptr;
}

llvm::Value *LLVMCodeGenerator::specialFormGenCode(const Symbol &SFSymbol,
                                                   const List *Args) {
  if (SFSymbol.name() == "deffn") {
    return defFnGenCode(Args);
  }
  return nullptr;
}

llvm::Value *LLVMCodeGenerator::fnCallGenCode(const Symbol &FuncSymbol,
                                              const List *Args) {
  llvm::Function *Func = this->Module->getFunction(FuncSymbol.name());
  if (!Func)
    return nullptr;
  std::vector<llvm::Value *> ArgsV;
  for (Cons *Curr = Args->Head; Curr; Curr = Curr->Next) {
    ArgsV.push_back(genCode(Curr->Obj));
    if (ArgsV.size() > FuncSymbol.object()->Val->SlFunction->numArgs())
      return nullptr;
  }
  return this->Builder.CreateCall(Func, ArgsV, "calltmp");
}

llvm::Function *LLVMCodeGenerator::defFnGenCode(const List *Args) {
  Symbol *FuncSymbol = Args->Head->Obj->Val->Symbol;
  List *Rest = listTail(Args);
  List *FuncArgs = Rest->Head->Obj->Val->List;
  Object *Body = Rest->Head->Next->Obj;
  std::vector<llvm::Type *> Ints(
      listCount(FuncArgs), llvm::Type::getInt64Ty(llvm::getGlobalContext()));
  llvm::FunctionType *FT = llvm::FunctionType::get(
      llvm::Type::getInt64Ty(llvm::getGlobalContext()), Ints, false);
  llvm::Function *Func = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, FuncSymbol->name(), this->Module);
  // Set names for all arguments.
  Cons *Curr = FuncArgs->Head;
  for (auto &Arg : Func->args()) {
    if (Curr == nullptr)
      return nullptr;
    std::string ArgName = Curr->Obj->Val->Symbol->name();
    Arg.setName(ArgName);
    Curr = Curr->Next;
  }

  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", Func);
  this->Builder.SetInsertPoint(BB);
  // Record the function arguments in the NamedValues map.
  this->NamedValues.clear();
  for (auto &Arg : Func->args())
    NamedValues[Arg.getName()] = &Arg;
  if (llvm::Value *RetVal = genCode(Body)) {
    // Finish off the function.
    Builder.CreateRet(RetVal);

    // Validate the generated code, checking for consistency.
    verifyFunction(*Func);

    return Func;
  }
  Func->eraseFromParent();
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
    fprintf(stderr, "Error Obj: %s, Type: %s\n", objToStr(Obj).c_str(),
            sltypeToStr(Obj->Type).c_str());
    return nullptr;
  }
}

const std::queue<SemanticError *> LLVMCodeGenerator::semanticErrors() {
  return this->SemanticErrors;
}
