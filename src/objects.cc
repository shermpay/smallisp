#include "Objects.h"

namespace sl {

// -------- Definitions for Int -----------
const sl::Int *sl::Int::kZero = new sl::Int(0);
const sl::Int *sl::Int::kOne = new sl::Int(1);

Type Int::GetType() const { return sl::Type::kInt; }

// -------- Definition for Symbol ----------
static std::unordered_map<std::string, Symbol *> symbol_pool_;

Type Symbol::GetType() const { return sl::Type::kSymbol; }

Symbol *Symbol::Get(const std::string &name) {
  Symbol *sym;
  auto found = symbol_pool_.find(name);
  if (found == symbol_pool_.end()) {
    sym = new Symbol(name);
    symbol_pool_.insert({name, sym});
  } else {
    sym = found->second;
  }
  return sym;
}

} // namespace sl
