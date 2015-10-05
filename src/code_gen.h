/* -*- mode: C++ -*- */
#ifndef _CODE_GEN_DEF
#define _CODE_GEN_DEF

#include "sltypes.h"

#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>

#include <map>
#include <queue>


enum SemanticErrorType {
  SE_UseBeforeDecl,
};

struct SemanticError {
  SemanticErrorType Type;
  const Object *Obj;

public:
  SemanticError(const SemanticErrorType &Type, const Object *Obj) {
    this->Type = Type;
    this->Obj = Obj;
  }
  std::string toStr() {
    const char *Str = objToStr(this->Obj);
    return std::string(Str);
  }
};

// Defines the interface for IR code.
class IRCode {};


/* Code Generator defines an interface.
 */
class CodeGenerator {
public:
  // Returns an error value.
  // llvm::Value *errorV(const char *Str) {
  //   return llvm::Error(Str);
  // }


  // llvm::Value *numberGenCode(const Object *Obj) {
  //   return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APInt(Obj->Val->SlNum));
  // }

  virtual llvm::Value *symbolGenCode(const Object *Obj) = 0;
  virtual llvm::Value *genCode(const Object *Obj) = 0;
  virtual llvm::Value *genBuiltinCode(const char *SymbolName, 
                                      int Nargs, 
                                      const Object **Args) = 0;
  virtual const std::queue<SemanticError*> semanticErrors() = 0;
  virtual ~CodeGenerator() {}
};

class LLVMCodeGenerator : CodeGenerator {
public:
  // Returns an error value.
  // llvm::Value *errorV(const char *Str) {
  //   return llvm::Error(Str);
  // }


  // llvm::Value *numberGenCode(const Object *Obj) {
  //   return llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APInt(Obj->Val->SlNum));
  // }
  LLVMCodeGenerator(const llvm::Module *Module) : Builder(llvm::getGlobalContext()){
    this->Module = Module;
  }
  
  virtual ~LLVMCodeGenerator() {
    delete this->Module;
  };

  virtual llvm::Value *symbolGenCode(const Object *Obj);
  virtual llvm::Value *genCode(const Object *Obj);
  virtual llvm::Value *genBuiltinCode(const char *SymbolName, 
                                      int Nargs, 
                                      const Object **Args);
  virtual const std::queue<SemanticError*> semanticErrors();


private:
  const llvm::Module *Module;
  llvm::IRBuilder<> Builder;
  std::map<std::string, llvm::Value*> NamedValues;
  std::queue<SemanticError*> SemanticErrors;
  

};

#endif
