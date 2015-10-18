//===- smallisp/objects.h - Smallisp objects definition --*- mode: C++ -*-=== //

#ifndef _OBJECTS_DEF
#define _OBJECTS_DEF

#include <string>
#include <unordered_map>
#include <iostream>

namespace sl {

// Type defines all the primitive Smallisp Types.
enum class Type {
  kInt,
  kSymbol,
  kCons,
  kList,
};

// Object is an Abstract Base Class.
// It defines the interface to all Smallisp Objects.
class Object {
 public:
  // Return the type of the object for introspection.
  virtual Type GetType() const = 0;
  virtual ~Object(){};
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
  virtual Type GetType() const;
  inline long value() const { return this->value_; };
  static Int *Get(const long &x);

 private:
  const long value_;
  Int() = delete;
  Int(const long &x) : value_(x){};
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
  virtual Type GetType() const;
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

}  // namespace sl

#endif
