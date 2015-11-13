#include "builtins.h"

namespace sl {

namespace builtins {

const Symbol *kAdd(void) {
  static auto add_sym = Symbol::Get("add");
  return add_sym;
}
const Symbol *kSub(void) {
  static auto sub_sym = Symbol::Get("sub");
  return sub_sym;
}
const Symbol *kMul(void) {
  static auto mul_sym = Symbol::Get("mul");
  return mul_sym;
}

const Symbol *kDiv(void) {
  static auto div_sym = Symbol::Get("div");
  return div_sym;
}

const Symbol *kCons(void) {
  static auto cons_sym = Symbol::Get("cons");
  return cons_sym;
}

}  // namespace builtins
}  // namespace sl
