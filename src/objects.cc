#include "objects.h"

namespace sl {

// -------- Definitions for Int -----------
std::unordered_map<long, Int *> Int::pool_;
const Int *Int::kZero = Int::Get(0);
const Int *Int::kOne = Int::Get(1);

Type Int::GetType() const { return sl::Type::kInt; }

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
Type Symbol::GetType() const { return sl::Type::kSymbol; }

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
