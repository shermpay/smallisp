#include "code_gen.h"
#include "parser.h"
#include "llvm_jit.h"

#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>

#include <editline/readline.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

std::unique_ptr<llvm::Module> InitializeModule(void) {
  auto Module =
      llvm::make_unique<llvm::Module>("my cool jit", llvm::getGlobalContext());
  // Module->setDataLayout(TheJIT->getTargetMachine().createDataLayout());
  return Module;
}

int StartRepl() {
  puts("Smallisp REPL V0.9.");
  puts("Press Ctrl-C to exist\n");
  llvm::raw_os_ostream OutStream(std::cout);
  auto BuiltinModule = InitializeModule();
  LLVMCodeGenerator *CodeGen = new LLVMCodeGenerator(BuiltinModule.get());
  llvm::ExecutionEngine *EE =
      llvm::EngineBuilder(std::move(BuiltinModule)).create();
  Object *Expr = static_cast<Object *>(malloc(sizeof(*Expr)));
  while (1) {
    char *Input = readline("slisp> ");
    add_history(Input);
    parseString(Input, &Expr);
    printf("Object: %s\n", objToStr(Expr).c_str());
    auto TheModule = InitializeModule();
    CodeGen = new LLVMCodeGenerator(TheModule.get());
    llvm::Value *Val = CodeGen->genCode(Expr);
    OutStream << "IR: ";
    if (Val) {
      Val->print(OutStream);
      OutStream << "\n";
      EE->addModule(std::move(TheModule));
    } else {
      OutStream << "nil\n";
    }
    OutStream.flush();
    free(Input);
  }
  return 0;
}
