//===- smallisp/int.h - Smallisp number interface --*- mode: C++ -*-=== //
#ifndef _NUMBER_DEF
#define _NUMBER_DEF

#include "object.h"

namespace sl {

// Number Interface
// Number has to provide the following primitive number accessors.
//   char_value: return a C char
//   int_value: return a C int
//   long_value: return a C long
// A Number allows the following arithmetic operations:
//   addition(+), subtraction(-), multiplication(*), division(/)
// It also implements the Comparable interface:
//   int Cmp(const Number &other) const
//     return x (x == 0) : if this == other
//     return x (x < 0)  : if this < other
//     return x (x > 0)  : if this > other
// Number classes are recommended to implement conversions from Number
class Number : public Object {
 public:
  DEF_TYPE_OBJ("Number");

  virtual Type &GetType() const override { return Number::TypeObj(); };

  // Number Interface
  // Accessors
  // virtual char char_value(void) const;
  // virtual int int_value(void) const;
  virtual long long_value(void) const = 0;

  // Arithmetic Operators
  virtual const Number &operator+(const Number &) const = 0;
  virtual const Number &operator-(const Number &) const = 0;
  virtual const Number &operator*(const Number &) const = 0;
  virtual const Number &operator/(const Number &) const = 0;

  // Comparable
  virtual int Cmp(const Number &) const = 0;
};

// Must be inline, else it would complain multiple definition
inline bool operator==(const Number &lhs, const Number &rhs) {
  return lhs.Cmp(rhs) == 0;
}

inline bool operator!=(const Number &lhs, const Number &rhs) {
  return lhs.Cmp(rhs) != 0;
}

}  // namespace sl

#endif
