#include "symbol.h"

#include <unordered_map>
#include "visitor.h"

namespace sl {

// -------- Definition for Symbol ----------
static std::unordered_map<std::string, Symbol &> &SymbolPool(void) {
  static std::unordered_map<std::string, Symbol &> pool;
  return pool;
};

const Symbol *Symbol::Get(const std::string &name) {
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

const Symbol &Symbol::Val(const std::string &name) {
  return *Symbol::Get(name);
}

const Object &Symbol::Accept(Visitor &v) const { return v.Visit(*this); }

}  // namespace sl
