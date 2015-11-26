#include "int.h"

#include <unordered_map>
#include "visitor.h"

namespace sl {
// -------- Definitions for Int -----------
static std::unordered_map<long, const Int &> &IntPool(void) {
  static std::unordered_map<long, const Int &> pool;
  return pool;
};

const Int *Int::Get(const long &x) {
  auto found = IntPool().find(x);
  if (found == IntPool().end()) {
    const Int *result = new Int(x);
    IntPool().insert({x, *result});
    return result;
  } else {
    const Int *result = &(found->second);
    return result;
  }
}

const Int &Int::Val(const long &x) { return *Int::Get(x); };

const Object *Int::Accept(Visitor &v) const { return v.Visit(*this); }

}  // namespace sl
