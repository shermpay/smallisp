#include "Objects.h"

namespace sl {

static std::unordered_map<std::string, Symbol *> SymbolPool;

Symbol *Symbol::get(const std::string &Name) {
  Symbol *Sym;
  auto Found = SymbolPool.find(Name);
  if (Found == SymbolPool.end()) {
    Sym = new Symbol(Name);
    SymbolPool.insert({Name, Sym});
  } else {
    Sym = Found->second;
  }
  return Sym;
}

namespace objects {

inline Value *newValue() { return new Value(); }

Object *newObject(Type Type, Value *X) {
  Object *Obj = new Object();
  Obj->Type = Type;
  Obj->Val = X;
  return Obj;
}

Object *newInt(const sl::Int &X) {
  Value *Val = newValue();
  Val->Int = X;
  return newObject(sl::Type::Int, Val);
}

Object *newSymbol(const std::string &X) {
  Value *Val = newValue();
  Val->Symbol = sl::Symbol::get(X);
  return newObject(sl::Type::Symbol, Val);
}

} // namespace sl
} // namespace objects
