#ifndef _ARRAY_DEF
#define _ARRAY_DEF

#include "object.h"
#include "error.h"

namespace sl {

class Array : public Object {
public:
  DEF_TYPE_OBJ("Array");

  Array(const std::size_t size, const Object **values)
      : size_(size), values_(values){};

  Array(std::initializer_list<const Object *> init_list);

  std::size_t size(void) const { return size_; }
  const Object **values(void) const { return values_; };

  // Object Interface
  const class Type &GetType() const override { return Array::TypeObj(); };
  bool IsEqual(const Object &) const override;
  std::size_t Hashcode(void) const override;
  const std::string Str(void) const override;
  const Object &Accept(class Visitor &) const override;

  const Object &operator[](const std::size_t idx);

private:
  const std::size_t size_;
  // Array of Object pointers
  const Object **values_;
  const Object **InitListHelper(std::initializer_list<const Object *>);
};

} // namespace sl

#endif
