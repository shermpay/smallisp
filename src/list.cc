/*
  Sherman Pay Jing Hao
  Tuesday, 10. June 2014

  Smallisp List
  -------------
*/
#include "list.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <vector>
#include <sstream>
#include <iostream>

namespace sl {

// ---------------- ConsC ----------------
bool ConsC::IsEqual(const Object &o) const {
  return (*this) == dynamic_cast<const ConsC &>(o);
};

bool operator==(const ConsC &lhs, const ConsC &rhs) {
  return lhs.car()->IsEqual(*(rhs.car())) && lhs.cdr()->IsEqual(*(rhs.cdr()));
}

bool operator!=(const ConsC &lhs, const ConsC &rhs) { return !(lhs == rhs); }

// ---------------- Lists ----------------

// nil/empty list
const List *kNil = new List(nullptr);
const List *List::kEmpty = kNil;

List::~List(void){
    // const List *rest = this->Rest();
    // delete rest;
};

bool List::IsEqual(const Object &o) const {
  if (o.GetType() != sl::Type::kList)
    return false;
  return (*this) == dynamic_cast<const List &>(o);
}

const std::string List::Str(void) const {
  std::stringstream sstream;
  sstream.put('(');
  for (ListIterator iter = this->begin(); iter != this->end(); iter++) {
    const Object &o = *iter;
    sstream << o.Str();
    sstream.put(' ');
  }
  std::string str = sstream.str();
  str[str.length() - 1] = ')';
  return str;
}

const Object *List::First() const {
  if (this == kNil) {
    return kNil;
  }
  return this->head_->car();
}

const List *List::Rest(void) const {
  assert(this->head()->cdr()->GetType() == sl::Type::kList);
  return (dynamic_cast<const List *>(this->head()->cdr()));
}

size_t List::Count(void) const {
  size_t count = 0;
  const ConsC *curr = this->head();
  while (curr) {
    count++;
    curr = (dynamic_cast<const List *>(curr->cdr()))->head();
  }
  return count;
}

// ---------------- List Operators ----------------
bool operator==(const List &lhs, const List &rhs) {
  if (&lhs != kNil && &rhs != kNil) {
    return lhs.head()->IsEqual(*(rhs.head()));
  } else if (&lhs == kNil && &rhs == kNil) {
    return true;
  } else {
    return false;
  }
}

bool operator!=(const List &lhs, const List &rhs) { return !(lhs == rhs); }

// Cons function
const ConsC *Cons(const Object *o1, const Object *o2) {
  const ConsC *cell = new ConsC(o1, o2);
  return cell;
}

const List *Cons(const Object *o1, const List *o2) {
  const ConsC *cell = new ConsC(o1, o2);
  return new List(cell);
}

} // namespace sl
