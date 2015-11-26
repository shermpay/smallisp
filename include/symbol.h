//===- smallisp/symbol.h - Smallisp symbol objects --*- mode: C++ -*-===
#ifndef _SYMBOL_DEF
#define _SYMBOL_DEF

#include "object.h"

namespace sl {
// Smallisp Symbol Objects
// Construction of Symbol objects is done via the Symbol::get(std::string)
// static function.
class Symbol : public Object {
 public:
  // static std::unordered_map<std::string, Symbol *> pool_;
  TYPE_OBJ_FN("Symbol");

  virtual ~Symbol(){};

  // Implement Object
  const Type &GetType(void) const override { return Symbol::TypeObj(); };
  // We do address equality because Symbols are pooled.
  bool IsEqual(const Object &o) const override { return this->IsEqual(&o); };
  bool IsEqual(const Object *o) const override { return this == o; };
  std::size_t Hashcode(void) const override {
    std::hash<std::string> hash_fn;
    return hash_fn(name());
  }
  virtual const std::string Str(void) const override { return name_; };

  inline const std::string name() const { return this->name_; };
  // Use this for constructing symbols.
  static const Symbol *Get(const std::string &name);
  static const Symbol &Val(const std::string &name);
  const Object *Accept(Visitor &) const override;

 private:
  const std::string name_;

 private:
  Symbol() = delete;
  Symbol(const std::string &name) : name_(name) {}
};
template <>
class HashFn<Symbol> {
 public:
  std::size_t operator()(const Symbol &obj) const { return obj.Hashcode(); }
};

inline bool operator==(const Symbol &lhs, const Symbol &rhs) {
  return &lhs == &rhs;
}

inline bool operator!=(const Symbol &lhs, const Symbol &rhs) {
  return !(lhs == rhs);
}

// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const Symbol &o, std::ostream *os) { *os << o.Str(); };

}  // namespace sl
#endif
