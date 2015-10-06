/* -*- mode: C++ -*- */
#ifndef _CODE_GEN_DEF
#define _CODE_GEN_DEF

#include "builtins.h"
#include "list.h"
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
  std::string toStr() { return objToStr(this->Obj); }
};

// Defines the interface for IR code.
class IRCode {};

/* Code Generator defines an interface.
 */
class CodeGenerator {
public:
  virtual llvm::Value *symbolGenCode(const Object *Obj) = 0;
  virtual llvm::Value *genCode(const Object *Obj) = 0;
  virtual llvm::Value *callGenCode(const Object *Obj) = 0;
  virtual llvm::Value *builtinGenCode(const std::string &SymbolName,
                                      const List *Args) = 0;
  virtual const std::queue<SemanticError *> semanticErrors() = 0;
  virtual ~CodeGenerator() {}
};

class LLVMCodeGenerator : CodeGenerator {
public:
  LLVMCodeGenerator(const llvm::Module *Module)
      : Builder(llvm::getGlobalContext()) {
    this->Module = Module;
  }

  virtual ~LLVMCodeGenerator() { delete this->Module; };

  virtual llvm::Value *symbolGenCode(const Object *Obj);
  virtual llvm::Value *genCode(const Object *Obj);
  virtual llvm::Value *callGenCode(const Object *Obj);
  virtual llvm::Value *builtinGenCode(const std::string &SymbolName,
                                      const List *Args);
  virtual const std::queue<SemanticError *> semanticErrors();

private:
  const llvm::Module *Module;
  llvm::IRBuilder<> Builder;
  std::map<std::string, llvm::Value *> NamedValues;
  std::queue<SemanticError *> SemanticErrors;
};

#endif
