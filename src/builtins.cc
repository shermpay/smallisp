#include "builtins.h"

namespace sl {

namespace builtins {

Environment Defns(void) {
  static Environment defns = {
      {Symbol::Get("nil"), NIL},       {Symbol::Get("true"), &TRUE},
      {Symbol::Get("false"), &FALSE},  {Symbol::Get("add"), new Add()},
      {Symbol::Get("sub"), new Sub()}, {Symbol::Get("mul"), new Mul()},
      {Symbol::Get("div"), new Div()}, {Symbol::Get("eq"), new Eq()},
      {Symbol::Get("ne"), new Ne()},
  };
  return defns;
}
}  // namespace builtins
}  // namespace sl
