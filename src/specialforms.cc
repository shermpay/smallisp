#include "specialforms.h"
#include "symbol.h"

namespace sl {

namespace specialforms {
using SFTable = std::unordered_map<const Symbol *, SFKind>;
const Symbol &kDef = Symbol::Val("def");
const Symbol &kUnsafeSet = Symbol::Val("set!");
const Symbol &kLambda = Symbol::Val("lambda");
const Symbol &kIf = Symbol::Val("if");
const Symbol &kFunc = Symbol::Val("func");

static SFTable kSFTable = {
    {&kDef, SFKind::kDef},       {&kUnsafeSet, SFKind::kUnsafeSet},
    {&kLambda, SFKind::kLambda}, {&kIf, SFKind::kIf},
    {&kFunc, SFKind::kFunc},
};

bool IsSpecialForm(const Symbol &sym) {
  const auto &iter = kSFTable.find(&sym);
  return (iter != kSFTable.end());
};

SFKind GetKind(const Symbol &sym) {
  const auto &iter = kSFTable.find(&sym);
  if (iter == kSFTable.end()) {
    return SFKind::kInvalidSF;
  } else {
    return iter->second;
  }
};

}  // namespace specialforms
}  // namespace sl
