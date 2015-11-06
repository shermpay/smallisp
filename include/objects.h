//===- smallisp/objects.h - Smallisp objects definition --*- mode: C++ -*-=== //

#ifndef _OBJECTS_DEF
#define _OBJECTS_DEF

#include <string>
#include <unordered_map>
#include <functional>

#include <iostream>

namespace sl {

// Type defines all the primitive Smallisp Types.
enum class Type {
  kInt,
  kSymbol,
  kCons,
  kList,
  kFunction,
};

std::ostream &operator<<(std::ostream &os, const Type &type);

// Object is a pure Abstract Base Class.
// It defines the interface to all Smallisp Objects.
class Object {
 public:
  virtual ~Object(){};
  // Return the type of the object for introspection.
  virtual Type GetType() const = 0;
  // Value equality between 2 smallisp objects.
  virtual bool IsEqual(const Object &) const = 0;
  virtual bool IsEqual(const Object *) const = 0;
  // Hash code of this object
  virtual std::size_t Hashcode(void) const = 0;
  // Str should return a human readable std::string object
  virtual const std::string Str(void) const = 0;
};
template <class T>
class ObjectHashFn;

template <>
class ObjectHashFn<Object> {
 public:
  std::size_t operator()(const Object &obj) const { return obj.Hashcode(); }
};

template <>
class ObjectHashFn<Object *> {
 public:
  std::size_t operator()(const Object *obj) const { return obj->Hashcode(); }
};

// For debugging
// There is bug in gtest which requires duplication of PrintTo code
// ::testing::PrintToString will always NOT use the base class PrintTo,
// instead it uses the default PrintTo, which prints bytes
// Bug Location:
// https://code.google.com/p/googletest/issues/detail?id=443
inline void PrintTo(const Object &o, std::ostream *os) { *os << o.Str(); };

// Int representation
class Int : public Object {
 public:
  virtual ~Int(){};

  // Object functions
  virtual Type GetType() const override { return sl::Type::kInt; };
  // We do address equality because Ints are pooled.
  virtual bool IsEqual(const Object &o) const override {
    return this->IsEqual(&o);
  };
  virtual bool IsEqual(const Object *o) const override { return this == o; };
  virtual std::size_t Hashcode(void) const override { return value(); }
  virtual const std::string Str(void) const override {
    return std::to_string(this->value_);
  };

  inline long value() const { return this->value_; };
  static Int *Get(const long &x);

  // operators
  inline Int *operator+(const Int *oi) {
    return Int::Get(this->value() + oi->value());
  };

  inline Int *operator-(const Int *oi) {
    return Int::Get(this->value() - oi->value());
  };

  inline Int *operator*(const Int *oi) {
    return Int::Get(this->value() * oi->value());
  };

  inline Int *operator/(const Int *oi) {
    return Int::Get(this->value() / oi->value());
  };

 private:
  const long value_;
  Int() = delete;
  Int(const long &x) : value_(x){};
};

inline bool operator==(const Int &lhs, const Int &rhs) { return &lhs == &rhs; };

inline bool operator!=(const Int &lhs, const Int &rhs) {
  return !(lhs == rhs);
};

// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const Int &o, std::ostream *os) { *os << o.Str(); };

// inline std::ostream &operator<<(std::ostream &os, const Int &o) {
//   return os << o.Str();
// };

// Smallisp Symbol Objects
// Construction of Symbol objects is done via the Symbol::get(std::string)
// static function.
class Symbol : public Object {
 public:
  // static std::unordered_map<std::string, Symbol *> pool_;
  virtual ~Symbol() { std::cout << "~Symbol" << std::endl; };

  // Implement Object
  virtual Type GetType() const override { return sl::Type::kSymbol; };
  // We do address equality because Symbols are pooled.
  virtual bool IsEqual(const Object &o) const override {
    return this->IsEqual(&o);
  };
  virtual bool IsEqual(const Object *o) const override { return this == o; };
  virtual std::size_t Hashcode(void) const override {
    std::hash<std::string> hash_fn;
    return hash_fn(name());
  }
  virtual const std::string Str(void) const override { return name_; };

  inline const std::string name() const { return this->name_; };
  // Use this for constructing symbols.
  static Symbol *Get(const std::string &name);

 private:
  const std::string name_;

 private:
  Symbol() = delete;
  Symbol(const std::string &name) : name_(name) {}
};

inline bool operator==(const Symbol &lhs, const Symbol &rhs) {
  return &lhs == &rhs;
}

inline bool operator!=(const Symbol &lhs, const Symbol &rhs) {
  return !(lhs == rhs);
}

// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const Symbol &o, std::ostream *os) { *os << o.Str(); };

}  // namespace sl

#endif
