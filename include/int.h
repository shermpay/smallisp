//===- smallisp/int.h - Smallisp int objects --*- mode: C++ -*-=== //

#ifndef _INT_DEF
#define _INT_DEF

#include "number.h"

namespace sl {

// Int representation
// TODO: Overflow checks
class Int : public Number {
 public:
  DEF_TYPE_OBJ("Int");

  Int(const Number &x) : value_(x.long_value()){};
  Int(const Int &x) : value_(x.value()){};
  Int(const Int &&x) : value_(x.value()){};
  Int(const long &x) : value_(x){};

  virtual ~Int(){};

  static const Int &Val(const long &x);
  inline long value() const { return this->value_; };
  // Object functions
  virtual Type &GetType() const override { return Int::TypeObj(); };
  bool IsEqual(const Object &o) const override;
  bool IsEqual(const Object *o) const override;
  std::size_t Hashcode(void) const override {
    std::hash<long> fn;
    return fn(value());
  }
  const std::string Str(void) const override {
    return std::to_string(this->value_);
  };
  const Object *Accept(Visitor &) const override;

  long long_value(void) const override { return value_; }

  // Arithmetic Operators
  inline const Number &operator+(const Number &oi) const override {
    return Int::Val(this->value() + oi.long_value());
  };

  inline const Number &operator-(const Number &oi) const override {
    return Int::Val(this->value() - oi.long_value());
  };

  inline const Number &operator*(const Number &oi) const override {
    return Int::Val(this->value() * oi.long_value());
  };

  inline const Number &operator/(const Number &oi) const override {
    assert(oi.long_value() != 0 && "div by 0");
    return Int::Val(this->value() / oi.long_value());
  };

  inline int Cmp(const Number &o) const override {
    return this->long_value() - o.long_value();
  }

 private:
  const long value_;
  Int() = delete;
  Int &operator=(const Int &) = delete;    // Copy Assignment
  Int &operator=(const Int &&x) = delete;  // Move Assignment
};

// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const Int &o, std::ostream *os) { *os << o.Str(); };

}  // namespace sl

#endif
