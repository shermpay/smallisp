#include "int.h"

#include <unordered_map>
#include "visitor.h"

namespace sl {
// -------- Definitions for Int -----------
const Int &Int::Val(const long &x) { return *new Int(x); };
bool Int::IsEqual(const Object &o) const {
  if (IsType<Number>(o)) {
    return this->Cmp(static_cast<const Number &>(o));
  }
  return false;
}

bool Int::IsEqual(const Object *o) const { return this->IsEqual(*o); }

const Object *Int::Accept(Visitor &v) const { return v.Visit(*this); }

}  // namespace sl
