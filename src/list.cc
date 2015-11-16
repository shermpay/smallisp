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

#include "utils.h"

namespace sl {

// ---------------- ConsC ----------------
bool ConsC::IsEqual(const Object &o) const {
  return (*this) == static_cast<const ConsC &>(o);
};

bool ConsC::IsEqual(const Object *o) const {
  return (*this) == *static_cast<const ConsC *>(o);
};

bool operator==(const ConsC &lhs, const ConsC &rhs) {
  return lhs.car()->IsEqual(*(rhs.car())) && lhs.cdr()->IsEqual(*(rhs.cdr()));
}

bool operator!=(const ConsC &lhs, const ConsC &rhs) { return !(lhs == rhs); }

// ---------------- Lists ----------------

// ListIterator
List::ListIterator &List::ListIterator::operator++() {
  assert(curr_->Rest()->GetType() == sl::Type::kList);
  curr_ = static_cast<const List *>(curr_->Rest());
  ++curr_count_;
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

const List *List::kEmpty = NIL;

const List *InitHelperPtr(std::initializer_list<const Object *> il) {
  const List *curr = NIL;
  for (auto ptr = il.end() - 1; ptr != il.begin() - 1; --ptr) {
    const Object *o = (*ptr);
    curr = Cons(o, curr);
  }
  return curr;
}

List::List(std::initializer_list<const Object *> il)
    : head_(InitHelperPtr(il)->head()) {}

List::~List(void){
    // const List *rest = this->Rest();
    // delete rest;
};

bool List::IsEqual(const Object &o) const {
  if (o.GetType() != sl::Type::kList) return false;
  return (*this) == static_cast<const List &>(o);
}

bool List::IsEqual(const Object *o) const { return this->IsEqual(*o); }

std::size_t List::Hashcode(void) const {
  std::size_t multiplier = 1;
  std::size_t result = 0;
  for (const auto &cur : *this) {
    result += cur.Hashcode() * multiplier;
    multiplier += 1;
  }
  return result;
}

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
  if (this == NIL) {
    return NIL;
  }
  return this->head_->car();
}

const Object *List::Rest(void) const {
  assert(this->head()->cdr()->GetType() == sl::Type::kList);
  return (static_cast<const List *>(this->head()->cdr()));
}

size_t List::Count(void) const {
  size_t count = 0;
  count++;
  if (this != NIL) count += static_cast<const List *>(this->Rest())->Count();
  return count;
}

// ---------------- List Operators ----------------
bool operator==(const List &lhs, const List &rhs) {
  if (!IsNil(&lhs) && !IsNil(&rhs)) {
    return lhs.head()->IsEqual(*(rhs.head()));
  } else if (IsNil(&lhs) && IsNil(&rhs)) {
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

// ---------------- Nil Definitions ----------------
static const std::string kDerefNilErrorMsg = "cannot dereference nil";

const ConsC *Nil::head(void) const {
  debug::ErrorWithTrace(kDerefNilErrorMsg);
  return nullptr;
};

const Object *Nil::First(void) const {
  return debug::ErrorWithTrace(kDerefNilErrorMsg);
};

const Object *Nil::Rest(void) const {
  return debug::ErrorWithTrace(kDerefNilErrorMsg);
};

}  // namespace sl
