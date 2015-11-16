#include "builtins.h"

namespace sl {

namespace builtins {

const Symbol *NilSym(void) {
  static auto nil_sym = Symbol::Get("nil");
  return nil_sym;
}

const Symbol *TrueSym(void) {
  static auto true_sym = Symbol::Get("true");
  return true_sym;
}

const Symbol *FalseSym(void) {
  static auto false_sym = Symbol::Get("false");
  return false_sym;
}

const Symbol *AddSym(void) {
  static auto add_sym = Symbol::Get("add");
  return add_sym;
}
const Symbol *SubSym(void) {
  static auto sub_sym = Symbol::Get("sub");
  return sub_sym;
}
const Symbol *MulSym(void) {
  static auto mul_sym = Symbol::Get("mul");
  return mul_sym;
}

const Symbol *DivSym(void) {
  static auto div_sym = Symbol::Get("div");
  return div_sym;
}

const Symbol *ConsSym(void) {
  static auto cons_sym = Symbol::Get("cons");
  return cons_sym;
}

}  // namespace builtins
}  // namespace sl
