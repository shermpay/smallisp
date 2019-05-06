//===- smallisp/void.h - Smallisp objects definition --*- mode: C++ -*-=== //

#ifndef _VOID_DEF
#define _VOID_DEF

#include "object.h"

namespace sl {

struct Void : public Object {
  DEF_TYPE_OBJ("void")
  // Return the type of the object for introspection.
  Type &GetType() const override { return Void::TypeObj(); };
  // Value equality between 2 smallisp objects.
  bool IsEqual(const Object &o) const override {
    std::cout << o.GetType().Str() << std::endl;
    assert(false && "IsEqual: void object");
    return false;
  };
  // Hash code of this object
  std::size_t Hashcode(void) const override {
    assert(false && "Hashcode: void object");
    return 0;
  };
  // Str should return a human readable std::string object
  const std::string Str(void) const override { return "void"; };
  static const Void *Get(void);
  const Object &Accept(Visitor &) const override;

 private:
  static Void *instance;
  Void(){};
};

inline void PrintTo(const Void &o, std::ostream *os) { *os << o.Str(); };

extern const Void &kVoid;

inline bool IsVoid(const Object &obj) { return &obj == &kVoid; };

inline bool IsVoid(const Object *obj) { return obj == &kVoid; };

}  // namespace sl
#endif
