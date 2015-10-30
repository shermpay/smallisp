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
#include <iterator>

namespace sl {

// ---------------- ConsC ----------------
bool ConsC::IsEqual(const Object &o) const {
  return (*this) == dynamic_cast<const ConsC &>(o);
};

bool ConsC::IsEqual(const Object *o) const {
  return (*this) == *dynamic_cast<const ConsC *>(o);
};

bool operator==(const ConsC &lhs, const ConsC &rhs) {
  return lhs.car()->IsEqual(*(rhs.car())) && lhs.cdr()->IsEqual(*(rhs.cdr()));
}

bool operator!=(const ConsC &lhs, const ConsC &rhs) { return !(lhs == rhs); }

// ---------------- Lists ----------------

// ListIterator
List::ListIterator &List::ListIterator::operator++() {
  assert(curr_->cdr()->GetType() == sl::Type::kList);
  curr_ = dynamic_cast<const ConsC *>(
      dynamic_cast<const List *>(curr_->cdr())->head());
  return *this;
}

List::ListIterator List::ListIterator::operator++(int) {
  this->operator++();
  ListIterator ret = *this;
  return ret;
}

// nil/empty list
Nil *Nil::instance = nullptr;
const Nil *Nil::Get(void) {
  if (!Nil::instance) Nil::instance = new Nil;
  return instance;
}

const List *kNil = Nil::Get();
const List *List::kEmpty = kNil;

const List *InitHelper(std::initializer_list<const Object> il) {
  const List *curr = kNil;
  for (auto ptr = il.end() - 1; ptr != il.begin() - 1; --ptr) {
    const Object &o = (*ptr);
    curr = Cons(&o, curr);
  }
  return curr;
}

const List *InitHelperPtr(std::initializer_list<const Object *> il) {
  const List *curr = kNil;
  for (auto ptr = il.end() - 1; ptr != il.begin() - 1; --ptr) {
    const Object *o = (*ptr);
    curr = Cons(o, curr);
  }
  return curr;
}

List::List(std::initializer_list<const Object> il)
    : head_(InitHelper(il)->head()) {}

List::List(std::initializer_list<const Object *> il)
    : head_(InitHelperPtr(il)->head()) {}

List::~List(void){
    // const List *rest = this->Rest();
    // delete rest;
};

bool List::IsEqual(const Object &o) const {
  if (o.GetType() != sl::Type::kList) return false;
  return (*this) == dynamic_cast<const List &>(o);
}

bool List::IsEqual(const Object *o) const { return this->IsEqual(*o); }

const std::string List::Str(void) const {
  std::stringstream sstream;
  sstream.put('(');
  ListIterator iter = this->begin();
  if (iter != this->end()) {
    const Object &o = *iter;
    sstream << o.Str();
    ++iter;
  }
  while (iter != this->end()) {
    sstream.put(' ');
    const Object &o = *iter;
    sstream << o.Str();
    ++iter;
  }
  sstream.put(')');
  return sstream.str();
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

}  // namespace sl
