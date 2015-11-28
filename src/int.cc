#include "int.h"

#include <unordered_map>
#include "visitor.h"

namespace sl {
// -------- Definitions for Int -----------
const Int &Int::Val(const long &x) { return *Int::Get(x); };
const Int *Int::Get(const long &x) { return new Int(x); };

bool Int::IsEqual(const Object &o) const {
  // Generalize to Number
  if (IsType<Int>(o)) {
    return this->Cmp(static_cast<const Int &>(o)) == 0;
  }
  return false;
}

bool Int::IsEqual(const Object *o) const { return this->IsEqual(*o); }

const Object *Int::Accept(Visitor &v) const { return v.Visit(*this); }

}  // namespace sl
