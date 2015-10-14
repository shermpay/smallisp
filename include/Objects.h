/* -*- mode: C++ -*- */
#ifndef _OBJECTS_DEF
#define _OBJECTS_DEF

#include "List.h"
#include <string>
#include <unordered_map>

namespace sl {

enum class Type {
  Int,
  Symbol,
  List,
};

struct Object;

// Int representation
typedef long Int;

// Smallisp Symbol Objects
class Symbol {
public:
  const std::string Name;
  Object *Obj;
  // Use this for constructing symbols.
  static Symbol *get(const std::string &Name);

private:
  Symbol(const std::string &Name) : Name(Name) {}
};

union Value {
  Int Int;

  Symbol *Symbol;
  // List *List;
};

struct Object {
  Type Type;
  Value *Val;
};

namespace objects {

Object *newObject(Type Type, Value *X);
Object *newInt(const Int &X);
Object *newSymbol(const std::string &X);

} // namespace objects
} // namespace sl

#endif
