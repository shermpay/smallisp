#ifndef _BYTE_DEF
#define _BYTE_DEF

#include "number.h"

namespace sl {

// Represents a byte(8 bits) sized value
class Byte : public Number {
public:
  DEF_TYPE_OBJ("Byte")

  static Byte &Val(const char &c) { return *(new Byte(c)); };
  Byte(const char &c) : value_(c){};
  char value() const { return value_; }

  const Type &GetType(void) const override { return Byte::TypeObj(); };
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

  // Number Interface
  long long_value(void) const override { return value_; };
  // TODO: Overflow check for all arithmetic operations
  const Number &operator+(const Number &o) const override {
    return Byte::Val(this->value() + o.long_value());
  };
  const Number &operator-(const Number &o) const override {
    return Byte::Val(this->value() - o.long_value());
  };
  const Number &operator*(const Number &o) const override {
    return Byte::Val(this->value() * o.long_value());
  };
  const Number &operator/(const Number &o) const override {
    return Byte::Val(this->value() / o.long_value());
  };

  // TODO: Overflow check
  inline int Cmp(const Number &o) const override {
    return this->value() - o.long_value();
  }

private:
  char value_;
};

// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const Byte &o, std::ostream *os) { *os << o.Str(); };

} // namespace sl

#endif
