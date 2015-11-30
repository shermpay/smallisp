#include "bool.h"

#include "visitor.h"

namespace sl {
// -------- Definition for Bool --------

const Bool &True(void) { return Bool::True(); };
const Bool &False(void) { return Bool::False(); };
const Object &Bool::Accept(Visitor &v) const { return v.Visit(*this); }
}  // namespace sl
