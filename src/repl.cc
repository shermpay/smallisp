#include "repl.h"

#include <iostream>

#include <editline/readline.h>

#include "treewalk_interp.h"
#include "reader.h"

namespace sl {

namespace repl {

static const std::string kPrompt = "sl> ";

int Start() {
  puts("Smallisp REPL v0.9");
  puts("Press Ctrl-D to exit");
  interp::Treewalker interp;
  for (;;) {
    interp.Print();  // For debugging
    std::cout << kPrompt;
    Reader reader(std::cin);
    const Object *expr = reader.ReadExpr();
    if (!expr) {
      if (!reader.error()) return 0;
      std::cout << reader.error() << std::endl;
      continue;
    }
    const Object *obj = interp.Eval(*expr);
    if (!obj) {
      continue;
    }
    std::cout << obj->Str() << std::endl;
  }
  return 1;
}

}  // namespace repl

}  // namespace sl
