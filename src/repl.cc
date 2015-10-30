#include "repl.h"

#include <iostream>

#include <editline/readline.h>

#include "reader.h"

namespace sl {

namespace repl {

static const std::string kPrompt = "sl> ";

int Start() {
  puts("Smallisp REPL v0.9");
  puts("Press Ctrl-D to exit");
  while (true) {
    std::cout << kPrompt;
    Reader reader(std::cin);
    const Object *obj = reader.ReadExpr();
    if (!obj) {
      std::cout << reader.error() << std::endl;
      return 0;
    }
    std::cout << obj->Str() << std::endl;
  }
  return 1;
}

}  // namespace repl

}  // namespace sl
