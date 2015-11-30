#include "array.h"

#include "visitor.h"

namespace sl {

const Object **Array::InitListHelper(
    std::initializer_list<const Object *> init_list) {
  const Object **arr = new const Object *[init_list.size()];
  int i = 0;
  for (auto &obj : init_list) {
    arr[i] = obj;
    ++i;
  }
  return arr;
}

Array::Array(std::initializer_list<const Object *> init_list)
    : size_(init_list.size()), values_(InitListHelper(init_list)){};

bool Array::IsEqual(const Object &other) const {
  if (!IsType<Array>(other)) return false;
  const Array &arr = static_cast<const Array &>(other);
  if (this->size() != arr.size()) return false;
  for (std::size_t i = 0; i < size(); i++) {
    const Object &obj = *values()[i];
    const Object &oth_obj = *arr.values()[i];
    if (!obj.IsEqual(oth_obj)) return false;
  }
  return true;
}

std::size_t Array::Hashcode(void) const {
  std::size_t code = 13;
  for (std::size_t i = 0; i < size(); i++) {
    code += (i + 1) * this->values()[i]->Hashcode();
  }
  return code;
}

const std::string Array::Str(void) const {
  std::string str;
  for (std::size_t i = 0; i < size(); i++) {
    const Object &obj = *(this->values()[i]);
    str.append(obj.Str());
  }
  return str;
}

const Object &Array::Accept(class Visitor &v) const { return v.Visit(*this); }

const Object &Array::operator[](const std::size_t idx) {
  if (idx < size()) return *values()[idx];
  return Error::Val("Array index out of bounds");
}

}  // namespace sl
