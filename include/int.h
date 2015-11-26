//===- smallisp/int.h - Smallisp int objects --*- mode: C++ -*-=== //

#ifndef _INT_DEF
#define _INT_DEF

#include "object.h"

namespace sl {

// Int representation
class Int : public Object {
 public:
  TYPE_OBJ_FN("Int");

  virtual ~Int(){};

  // Object functions
  virtual Type &GetType() const override { return Int::TypeObj(); };
  // We do address equality because Ints are pooled.
  virtual bool IsEqual(const Object &o) const override {
    return this->IsEqual(&o);
  };
  virtual bool IsEqual(const Object *o) const override { return this == o; };
  virtual std::size_t Hashcode(void) const override {
    std::hash<long> fn;
    return fn(value());
  }
  virtual const std::string Str(void) const override {
    return std::to_string(this->value_);
  };

  inline long value() const { return this->value_; };
  static const Int *Get(const long &x);
  static const Int &Val(const long &x);

  // operators
  inline const Int &operator+(const Int &oi) const {
    return Int::Val(this->value() + oi.value());
  };

  inline const Int &operator-(const Int &oi) const {
    return Int::Val(this->value() - oi.value());
  };

  inline const Int &operator*(const Int &oi) const {
    return Int::Val(this->value() * oi.value());
  };

  inline const Int &operator/(const Int &oi) const {
    assert(oi.value() != 0 && "div by 0");
    return Int::Val(this->value() / oi.value());
  };
  const Object *Accept(Visitor &) const override;

 private:
  const long value_;
  Int() = delete;
  Int(const Int &) = delete;               // Copy ctor
  Int(const Int &&x) = delete;             // Move ctor
  Int &operator=(const Int &) = delete;    // Copy assignment
  Int &operator=(const Int &&x) = delete;  // Move assignment
  Int(const long &x) : value_(x){};
};

inline bool operator==(const Int &lhs, const Int &rhs) { return &lhs == &rhs; };

inline bool operator!=(const Int &lhs, const Int &rhs) {
  return !(lhs == rhs);
};

// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const Int &o, std::ostream *os) { *os << o.Str(); };

}  // namespace sl

#endif
