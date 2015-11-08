#include "specialforms.h"

#include "objects.h"

namespace sl {

namespace specialforms {

using SFTable = std::unordered_map<const Symbol *, SFKind>;

static SFTable kSFTable = {
    {Symbol::Get("set!"), SFKind::kUnsafeSet},
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
