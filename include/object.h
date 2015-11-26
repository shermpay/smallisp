//===- smallisp/object.h - Smallisp objects definition --*- mode: C++ -*-=== //

#ifndef _OBJECT_DEF
#define _OBJECT_DEF

#include <cassert>

#include <iostream>
#include <functional>
#include <string>

namespace sl {
// Object is a pure Abstract Base Class.
// It defines the interface to all Smallisp Objects.
class Object {
 public:
  virtual ~Object(){};
  // Return the type of the object for introspection.
  virtual const class Type &GetType() const = 0;
  // Value equality between 2 smallisp objects.
  virtual bool IsEqual(const Object &) const = 0;
  virtual bool IsEqual(const Object *) const = 0;
  // Hash code of this object
  virtual std::size_t Hashcode(void) const = 0;
  // Str should return a human readable std::string object
  virtual const std::string Str(void) const = 0;
  // Visitor method
  virtual const Object *Accept(class Visitor &) const = 0;
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

class Type : public Object {
 public:
  static const Type &Inst(void) {
    static const Type *inst = new Type("Type");
    return *inst;
  }
  Type(const std::string &name) : name_(name){};
  const Type &GetType(void) const override { return Type::Inst(); }
  bool IsEqual(const Object *obj) const override { return this == obj; }
  bool IsEqual(const Object &obj) const override { return this->IsEqual(&obj); }
  std::size_t Hashcode(void) const override {
    return reinterpret_cast<std::size_t>(this);
  }
  const std::string Str(void) const override {
    return "<'" + name() + "' type object>";
  };
  const std::string &name(void) const { return name_; };
  const Object *Accept(Visitor &) const override {
    assert(false && "NOT IMPLEMENTED");
    return nullptr;
  };

 protected:
  Type(const Type &) = delete;
  Type &operator=(const Type &) = delete;

 private:
  Type() = delete;
  const std::string name_;
};

// TODO: Fix this
inline void PrintTo(const Type &o, std::ostream *os) { *os << o.Str(); };

inline const Type &TypeOf(const Object &obj) { return obj.GetType(); };

inline const Type &TypeOf(const Object *obj) { return TypeOf(*obj); };

template <typename T>
inline bool IsType(const Object &obj) {
  return TypeOf(obj) == T::TypeObj();
};

template <typename T>
inline bool IsType(const Object *obj) {
  return IsType<T>(*obj);
};

#define DEF_TYPE_OBJ(_name_)                \
  static Type &TypeObj(void) {              \
    static Type *_inst_ = new Type(_name_); \
    return *_inst_;                         \
  }

}  // namespace sl

#endif
