#include "objects.h"

#include <cassert>

#include <map>

namespace sl {
// Definitions for types
static const std::map<const Type, const std::string &> kTypeNameMap = {
    {Type::kInt, "Int"},
    {Type::kSymbol, "Symbol"},
    {Type::kCons, "Cons"},
    {Type::kList, "List"},
};

std::ostream &operator<<(std::ostream &os, const Type &type) {
  os << kTypeNameMap.at(type);
  return os;
}

// void PrintTo(const Object &o, std::ostream *os) { *os << ""; }
// void PrintTo(const Int &o, std::ostream *os) { *os << ""; }

// -------- Definitions for Int -----------
static std::unordered_map<long, Int &> &IntPool(void) {
  static std::unordered_map<long, Int &> pool;
  return pool;
};

Int &Int::Get(const long &x) {
  Int *result;
  auto found = IntPool().find(x);
  if (found == IntPool().end()) {
    result = new Int(x);
    IntPool().insert({x, *result});
  } else {
    result = &(found->second);
  }
  return *result;
}

// -------- Definition for Symbol ----------
static std::unordered_map<std::string, Symbol &> &SymbolPool(void) {
  static std::unordered_map<std::string, Symbol &> pool;
  return pool;
};

Symbol &Symbol::Get(const std::string &name) {
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
  return *sym;
}

}  // namespace sl
