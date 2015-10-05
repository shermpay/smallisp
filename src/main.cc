#include "repl.h"
#include "code_gen.h"

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <boost/program_options.hpp>

#include <cstdio>

int main(int argc, char *argv[]) {
  StartRepl();
  llvm::Module *Mod = new llvm::Module("my cool jit", llvm::getGlobalContext());
  LLVMCodeGenerator *CodeGen = new LLVMCodeGenerator(Mod);
}
