//===- smallisp/objects.h - Smallisp objects definition --*- mode: C++ -*-=== //

#ifndef _OBJECTS_DEF
#define _OBJECTS_DEF

#include <string>
#include <unordered_map>
#include <map>
#include <iostream>

namespace sl {

// Type defines all the primitive Smallisp Types.
enum class Type {
  kInt,
  kSymbol,
  kCons,
  kList,
};

std::ostream &operator<<(std::ostream &os, const Type &type);

// Object is an Abstract Base Class.
// It defines the interface to all Smallisp Objects.
class Object {
public:
  virtual ~Object(){};
  // Return the type of the object for introspection.
  virtual Type GetType() const = 0;
  // Value equality between 2 smallisp objects.
  virtual bool IsEqual(const Object *) const = 0;
  // Str should return a human readable std::string object
  virtual const std::string Str(void) const = 0;
};

// Int representation
class Int : public Object {
public:
  static std::unordered_map<long, Int *> pool_;
  // Constant for the sl::Int representing '0'
  static const Int *kZero;
  // Constant for the sl::Int representing '1'
  static const Int *kOne;
  virtual ~Int(){};

  // Object functions
  virtual Type GetType() const override { return sl::Type::kInt; };
  virtual bool IsEqual(const Object *) const override;
  virtual const std::string Str(void) const override {
    return std::to_string(this->value_);
  };

  inline long value() const { return this->value_; };
  static Int *Get(const long &x);

private:
  const long value_;
  Int() = delete;
  Int(const long &x) : value_(x){};
};

inline bool operator==(const Int &lhs, const Int &rhs) {
  return lhs.value() == rhs.value();
};

inline bool operator!=(const Int &lhs, const Int &rhs) {
  return !(lhs == rhs);
};

// Smallisp Symbol Objects
// Construction of Symbol objects is done via the Symbol::get(std::string)
// static function.
class Symbol : public Object {
public:
  static std::unordered_map<std::string, Symbol *> pool_;
  virtual ~Symbol() {
    std::cout << "~Symbol" << std::endl;
    delete this->obj_;
  };

  // Implement Object
  virtual Type GetType() const override { return sl::Type::kSymbol; };
  virtual bool IsEqual(const Object *o) const override;
  virtual const std::string Str(void) const override { return name_; };

  inline const std::string name() const { return this->name_; };
  inline const Object *obj() const { return this->obj_; };
  // Use this for constructing symbols.
  static Symbol *Get(const std::string &name);

private:
  const std::string name_;
  const Object *obj_;

private:
  Symbol() = delete;
  Symbol(const std::string &name) : name_(name) {}
};

inline bool operator==(const Symbol &lhs, const Symbol &rhs) {
  return lhs.name() == rhs.name();
}

inline bool operator!=(const Symbol &lhs, const Symbol &rhs) {
  return !(lhs == rhs);
}

} // namespace sl

#endif
