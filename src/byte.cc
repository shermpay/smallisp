#include "byte.h"

#include "visitor.h"

namespace sl {
bool Byte::IsEqual(const Object &o) const {
  if (IsType<Byte>(o)) {
    const Byte byte = *static_cast<const Byte *>(&o);
    return this->value() == byte.value();
  }
  return false;
}

bool Byte::IsEqual(const Object *o) const { return this->IsEqual(*o); }

const Object *Byte::Accept(Visitor &v) const { return v.Visit(*this); }

}  // namespace sl
