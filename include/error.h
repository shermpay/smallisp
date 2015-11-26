//===- smallisp/error.h - Smallisp error objects --*- mode: C++ -*-=== //

#ifndef _ERROR_DEF
#define _ERROR_DEF

#include "object.h"

namespace sl {
// Error class
class Error : public Object {
 public:
  TYPE_OBJ_FN("Error")
  Error(const std::string &msg) : msg_(msg){};
  // Return the type of the object for introspection.
  const sl::Type &GetType() const override { return Error::TypeObj(); };
  // Value equality between 2 smallisp objects.
  bool IsEqual(const Object &o) const override { return IsType<Error>(o); };
  bool IsEqual(const Object *o) const override { return IsEqual(*o); };
  // Hash code of this object
  std::size_t Hashcode(void) const override {
    assert(false && "Hashcode not implemented for Errors.");
    return 0;
  };
  // Str returns the error message
  const std::string Str(void) const override { return msg_; };
  bool operator==(const Error &rhs) { return this->IsEqual(rhs); }
  bool operator!=(const Error &rhs) { return !((*this) == rhs); }
  const Object *Accept(Visitor &) const override;

 private:
  const std::string msg_;
};
inline void PrintTo(const Error &o, std::ostream *os) { *os << o.Str(); };

}  // namespace sl
#endif
