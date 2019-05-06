#include "array.h"

#include <vector>

#include "list.h"
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

const Array &Array::FromList(const List &list) {
  std::size_t cap = 8, size = 0;
  const Object **values = new const Object *[cap];
  for (const auto &obj : list) {
    values[size] = &obj;
    ++size;
    if (size > cap)
      values = static_cast<const Object **>(
          realloc(values, sizeof(values) * cap * 2));
  }
  if (size < cap)
    values =
        static_cast<const Object **>(realloc(values, sizeof(values) * size));

  return *(new Array(size, values));
}

bool Array::IsEqual(const Object &other) const {
  if (!IsType<Array>(other)) return false;
  const Array &arr = static_cast<const Array &>(other);
  if (this->size() != arr.size()) return false;
  for (std::size_t i = 0; i < size(); ++i) {
    const Object &obj = (*this)[i];
    const Object &oth_obj = arr[i];
    if (!obj.IsEqual(oth_obj)) return false;
  }
  return true;
}

std::size_t Array::Hashcode(void) const {
  std::size_t code = 13;
  for (std::size_t i = 0; i < size(); ++i) {
    code += (i + 1) * this->values()[i]->Hashcode();
  }
  return code;
}

const std::string Array::Str(void) const {
  std::string str;
  str.push_back('[');
  if (size() > 0) {
    str.append((*this)[0].Str());
  }
  for (std::size_t i = 1; i < size(); ++i) {
    const Object &obj = (*this)[i];
    str.push_back(' ');
    str.append(obj.Str());
  }
  str.push_back(']');
  return str;
}

const Object &Array::Accept(class Visitor &v) const { return v.Visit(*this); }

const Object &Array::operator[](const std::size_t idx) const {
  if (idx < size()) return *(values()[idx]);
  return Error::Val("Array index out of bounds");
}

}  // namespace sl
