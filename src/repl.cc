#include "parser.h"
#include "code_gen.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_os_ostream.h>

#include <editline/readline.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>

int StartRepl() {
  puts("Smallisp REPL V0.9.");
  puts("Press Ctrl-C to exist\n");
  llvm::Module *Mod = new llvm::Module("my cool jit", llvm::getGlobalContext());
  LLVMCodeGenerator *CodeGen = new LLVMCodeGenerator(Mod);
  llvm::raw_os_ostream OutStream(std::cout);

  Object *Expr = static_cast<Object *>(malloc(sizeof(*Expr)));
  while (1) {
    char *Input = readline("slisp> ");
    add_history(Input);
    parseString(Input, &Expr);
    printf("Object: %s\n", objToStr(Expr).c_str());
    llvm::Value *Val = CodeGen->genCode(Expr);
    OutStream << "IR: ";
    if (Val) {
      Val->print(OutStream);
      OutStream << "\n";
    } else {
      OutStream << "nil\n";
    }
    OutStream.flush();
    free(Input);
  }
  return 0;
}
