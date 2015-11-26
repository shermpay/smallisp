//===- smallisp/objects.h - Smallisp objects definition --*- mode: C++ -*-=== //

#ifndef _BOOL_DEF
#define _BOOL_DEF

#include "object.h"

namespace sl {

class Bool : public Object {
 public:
  DEF_TYPE_OBJ("Bool");

  // Return the type of the object for introspection.
  const Type &GetType() const override { return Bool::TypeObj(); };
  // Value equality between 2 smallisp objects.
  virtual bool IsEqual(const Object &o) const override {
    return this->IsEqual(&o);
  };
  virtual bool IsEqual(const Object *o) const override { return this == o; };
  // Hash code of this object
  std::size_t Hashcode(void) const override {
    std::hash<bool> fn;
    return fn(value());
  };
  // Str should return a human readable std::string object
  const std::string Str(void) const override {
    return value_ ? "true" : "false";
  };
  bool value() const { return value_; };
  static const Bool &True(void) {
    static const Bool *t = new Bool(true);
    return *t;
  };
  static const Bool &False(void) {
    static const Bool *f = new Bool(false);
    return *f;
  };
  static const Bool *Get(bool val) { return val ? &True() : &False(); };
  explicit operator bool() const { return this->value(); };
  const Object *Accept(Visitor &) const override;

 private:
  Bool(bool val) : value_(val){};
  bool value_;
};

inline bool operator==(const Bool &lhs, const Bool &rhs) {
  return &lhs == &rhs;
};

inline bool operator!=(const Bool &lhs, const Bool &rhs) {
  return !(lhs == rhs);
};

const Bool &True(void);
#define TRUE (True())
const Bool &False(void);
#define FALSE (False())

// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const Bool &o, std::ostream *os) { *os << o.Str(); };

}  // namespace sl

#endif
