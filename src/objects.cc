#include "objects.h"

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
std::unordered_map<long, Int *> Int::pool_;

Int *Int::Get(const long &x) {
  Int *result;
  auto found = Int::pool_.find(x);
  if (found == Int::pool_.end()) {
    result = new Int(x);
    Int::pool_.insert({x, result});
  } else {
    result = found->second;
  }
  return result;
}

// -------- Definition for Symbol ----------
std::unordered_map<std::string, Symbol *> Symbol::pool_;

Symbol *Symbol::Get(const std::string &name) {
  Symbol *sym;
  auto found = Symbol::pool_.find(name);
  if (found == Symbol::pool_.end()) {
    sym = new Symbol(name);
    Symbol::pool_.insert({name, sym});
  } else {
    sym = found->second;
  }
  return sym;
}

}  // namespace sl
