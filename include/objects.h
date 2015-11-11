//===- smallisp/objects.h - Smallisp objects definition --*- mode: C++ -*-=== //

#ifndef _OBJECTS_DEF
#define _OBJECTS_DEF

#include <cassert>

#include <string>
#include <unordered_map>
#include <functional>

#include <iostream>

// Use the following code for stacktraces
// #include <execinfo.h>
// #define SIZE 8
//     void *backtrace_buf[SIZE];
//     int nptrs = backtrace(backtrace_buf, SIZE);
//     printf("backtrace returned %d addresses.\n", nptrs);
//     char **trace_strings = backtrace_symbols(backtrace_buf, nptrs);
//     if (trace_strings) {
//       for (int i = 0; i < nptrs; ++i) printf("%s\n", trace_strings[i]);
//     }

namespace sl {

// Type defines all the primitive Smallisp Types.
enum class Type {
  kInt,
  kSymbol,
  kCons,
  kList,
  kFunction,
  kVoid,
  kError,
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
class HashFn;

template <>
class HashFn<Object> {
 public:
  std::size_t operator()(const Object &obj) const { return obj.Hashcode(); }
};

template <>
class HashFn<Object *> {
 public:
  std::size_t operator()(const Object &obj) const { return obj.Hashcode(); }
};

inline bool operator==(const Object &lhs, const Object &rhs) {
  return lhs.IsEqual(rhs);
};

inline bool operator!=(const Object &lhs, const Object &rhs) {
  return !(lhs == rhs);
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
  static Int &Val(const long &x);

  // operators
  inline Int &operator+(const Int &oi) {
    return Int::Val(this->value() + oi.value());
  };

  inline Int &operator-(const Int &oi) {
    return Int::Val(this->value() - oi.value());
  };

  inline Int &operator*(const Int &oi) {
    return Int::Val(this->value() * oi.value());
  };

  inline Int &operator/(const Int &oi) {
    return Int::Val(this->value() / oi.value());
  };

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

// inline std::ostream &operator<<(std::ostream &os, const Int &o) {
//   return os << o.Str();
// };

// Smallisp Symbol Objects
// Construction of Symbol objects is done via the Symbol::get(std::string)
// static function.
class Symbol : public Object {
 public:
  // static std::unordered_map<std::string, Symbol *> pool_;
  virtual ~Symbol(){};

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
  static Symbol &Val(const std::string &name);

 private:
  const std::string name_;

 private:
  Symbol() = delete;
  Symbol(const std::string &name) : name_(name) {}
};
template <>
class HashFn<Symbol> {
 public:
  std::size_t operator()(const Symbol &obj) const { return obj.Hashcode(); }
};

inline bool operator==(const Symbol &lhs, const Symbol &rhs) {
  return &lhs == &rhs;
}

inline bool operator!=(const Symbol &lhs, const Symbol &rhs) {
  return !(lhs == rhs);
}

// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const Symbol &o, std::ostream *os) { *os << o.Str(); };

struct Void : public Object {
  // Return the type of the object for introspection.
  virtual Type GetType() const override { return Type::kVoid; };
  // Value equality between 2 smallisp objects.
  bool IsEqual(const Object &) const override {
    assert(false && "IsEqual: void object");
    return false;
  };
  bool IsEqual(const Object *o) const override { return this->IsEqual(*o); };
  // Hash code of this object
  std::size_t Hashcode(void) const override {
    assert(false && "Hashcode: void object");
    return 0;
  };
  // Str should return a human readable std::string object
  const std::string Str(void) const override { return "void"; };
  static const Void *Get(void);

 private:
  static Void *instance;
  Void(){};
};
inline void PrintTo(const Void &o, std::ostream *os) { *os << "void"; };

extern const Void *kVoid;

inline bool IsVoid(const Object &obj) { return &obj == kVoid; };

inline bool IsVoid(const Object *obj) { return obj == kVoid; };

// Error class
class Error : public Object {
 public:
  Error(const std::string &msg) : msg_(msg){};
  // Return the type of the object for introspection.
  sl::Type GetType() const override { return sl::Type::kError; };
  // Value equality between 2 smallisp objects.
  bool IsEqual(const Object &o) const override {
    return o.GetType() == Type::kError;
  };
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

 private:
  const std::string msg_;
};
inline void PrintTo(const Error &o, std::ostream *os) { *os << o.Str(); };

}  // namespace sl

#endif
