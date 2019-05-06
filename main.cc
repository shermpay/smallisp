#include <fstream>

#include "error.h"
#include "reader.h"
#include "repl.h"
#include "treewalk_interp.h"

namespace sl {}

class Object;

int main(int argc, char *argv[]) {
  if (argc == 1) {
    return sl::repl::Start();
  } else if (argc == 2) {
    const char *filename = argv[1];
    std::ifstream stream(filename);
    sl::Reader reader(stream);
    sl::interp::Treewalker interp;
    const sl::Object *expr = nullptr;
    while (!sl::IsType<sl::EOFError>(expr = &reader.ReadExpr())) {
      const sl::Object &obj = interp.Eval(*expr);
      if (sl::IsType<sl::Error>(obj)) {
        std::cout << "Error on line " << reader.linum() << ", col "
                  << reader.colnum() << ": " << reader.curr_line() << std::endl;
      }
      std::cout << obj.Str() << std::endl;
    }
  }
  return 0;
}
