#include "void.h"

#include "visitor.h"

namespace sl {
Void *Void::instance = nullptr;
const Void *Void::Get(void) {
  if (!Void::instance) Void::instance = new Void();
  return Void::instance;
}
const Void *kVoid = Void::Get();

const Object *Void::Accept(Visitor &v) const { return v.Visit(*this); }

}  // namespace sl
