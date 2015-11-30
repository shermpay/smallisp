#include "error.h"

#include "visitor.h"

namespace sl {
///////////////////////
// Error definitions //
///////////////////////
const Object &Error::Accept(Visitor &v) const { return v.Visit(*this); }
}  // namespace sl
