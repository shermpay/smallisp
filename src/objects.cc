#include "objects.h"

#include <cassert>

#include <map>

namespace sl {
// Definitions for types
static const std::map<const Type, const std::string> kTypeNameMap = {
    {Type::kInt, "Int"},
    {Type::kSymbol, "Symbol"},
    {Type::kCons, "Cons"},
    {Type::kList, "List"},
};

std::ostream &operator<<(std::ostream &os, const Type &type) {
  const auto &name_map = kTypeNameMap;
  const auto iter = name_map.find(type);
  if (iter != name_map.end())
    os << iter->second;
  else
    os << "Error: could not find name for type";

  return os;
}

// void PrintTo(const Object &o, std::ostream *os) { *os << ""; }
// void PrintTo(const Int &o, std::ostream *os) { *os << ""; }

// -------- Definitions for Int -----------
static std::unordered_map<long, Int &> &IntPool(void) {
  static std::unordered_map<long, Int &> pool;
  return pool;
};

Int *Int::Get(const long &x) {
  Int *result;
  auto found = IntPool().find(x);
  if (found == IntPool().end()) {
    result = new Int(x);
    IntPool().insert({x, *result});
  } else {
    result = &(found->second);
  }
  return result;
}

Int &Int::Val(const long &x) { return *Int::Get(x); };

// -------- Definition for Symbol ----------
static std::unordered_map<std::string, Symbol &> &SymbolPool(void) {
  static std::unordered_map<std::string, Symbol &> pool;
  return pool;
};

Symbol *Symbol::Get(const std::string &name) {
  Symbol *sym;
  assert(SymbolPool().bucket_count() > 0 &&
         "SymbolPool not initialized, bucket_count < 0");
  auto found = SymbolPool().find(name);
  if (found == SymbolPool().end()) {
    sym = new Symbol(name);
    SymbolPool().insert({name, *sym});
  } else {
    sym = &(found->second);
  }
  return sym;
}

Symbol &Symbol::Val(const std::string &name) { return *Symbol::Get(name); }

Void *Void::instance = nullptr;
const Void *Void::Get(void) {
  if (!Void::instance) Void::instance = new Void();
  return Void::instance;
}
const Void *kVoid = Void::Get();
}  // namespace sl
