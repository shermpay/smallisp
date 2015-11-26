//===- smallisp/list.h - Smallisp cons/list definition --*- mode: C++ -*-=== //
#ifndef _LIST_DEF
#define _LIST_DEF

#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <cstdio>

#include <iterator>
#include <vector>
#include <iostream>

#include "utils.h"
#include "object.h"

namespace sl {

// The definition of a Cons Cell in Smallisp
class ConsC : public Object {
 public:
  TYPE_OBJ_FN("Cons")
  ConsC(const Object *o1, const Object *o2) : car_(o1), cdr_(o2){};
  ~ConsC() noexcept {};

  // Implement Object
  const Type &GetType() const override { return ConsC::TypeObj(); };
  virtual bool IsEqual(const Object &o) const override;
  virtual bool IsEqual(const Object *o) const override;
  virtual std::size_t Hashcode(void) const override {
    return car()->Hashcode() * 13 + cdr()->Hashcode();
  };
  virtual const std::string Str(void) const override {
    return "(" + car_->Str() + " . " + cdr_->Str() + ")";
  };

  inline const Object *car() const { return this->car_; };
  inline const Object *cdr() const { return this->cdr_; };
  const Object *Accept(Visitor &v) const override;

 private:
  const Object *car_;
  const Object *cdr_;
};

bool operator==(const ConsC &lhs, const ConsC &rhs);
bool operator!=(const ConsC &lhs, const ConsC &rhs);
// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const ConsC &o, std::ostream *os) { *os << o.Str(); };

// The definition of a List in Smallisp
// nil == '() in smallisp.
// A list is a wrapper around a chain of cons where each cons cell
// has an Object in its car pointer and a list in its cdr pointer.
class List : public Object {
 public:
  class ListIterator {
   public:
    // typedefs for iterator
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;
    typedef Object value_type;
    typedef Object *pointer;
    typedef Object &reference;

    ListIterator(const List &list) : curr_(&list), curr_count_(0){};
    inline const List *curr() const { return this->curr_; };
    inline size_t curr_count(void) const { return this->curr_count_; };
    const Object &operator*() { return *(this->curr()->head()->car()); };
    ListIterator &operator++();
    ListIterator operator++(int);

    inline bool operator==(const ListIterator &rhs) const {
      return this->curr() == rhs.curr();
    }

    inline bool operator!=(const ListIterator &rhs) const {
      return !(*this == rhs);
    }

   private:
    const List *curr_;
    size_t curr_count_;
  };

  typedef ListIterator iterator;

  // Function: List::TypeObj
  TYPE_OBJ_FN("List")

  // The constant nil.
  static const List *kEmpty;

  // Constructor for wrapping a cons cell into a list type.
  List(const ConsC *cell) : head_(cell) {
    assert(!IsType<ConsC>(*(cell->cdr())));
  };

  // Constructor for creating lists of objects.
  List(std::initializer_list<const Object *> il);

  // Destructor for list
  ~List(void);

  // Implement object
  const Type &GetType(void) const override { return List::TypeObj(); };
  virtual bool IsEqual(const Object &o) const override;
  virtual bool IsEqual(const Object *o) const override;
  virtual std::size_t Hashcode(void) const override;
  virtual const std::string Str(void) const override;

  virtual inline const ConsC *head() const { return this->head_; };
  virtual const Object *First() const;

  // Returns the rest of the list
  virtual const Object *Rest(void) const;

  // Counts the number of elements in the list.
  virtual size_t Count(void) const;

  // Prints list in (a b c) format
  // void Print(void);

  // Iterator implementation
  iterator begin(void) { return iterator(*this); }
  iterator end(void) { return iterator(*kEmpty); }

  // const_iterator implementation
  iterator begin(void) const { return iterator(*this); }
  iterator end(void) const { return iterator(*kEmpty); }

  const Object *Accept(Visitor &v) const override;

 protected:
  List(){};

 private:
  const ConsC *head_;
};

// Special Singleton object Nil
struct Nil : public List {
 public:
  static const Nil *Get(void);
  const ConsC *head(void) const override;
  const Object *First() const override;
  const Object *Rest(void) const override;
  size_t Count(void) const override { return 0; };
  const std::string Str(void) const override { return "()"; };
  const Object *Accept(Visitor &v) const override;

 private:
  static Nil *instance;
  Nil(){};
  Nil(Nil const &) = delete;
  Nil &operator=(Nil const &) = delete;
};

bool operator==(const List &lhs, const List &rhs);

bool operator!=(const List &lhs, const List &rhs);

#define NIL (Nil::Get())

// TODO: Remove when bug in gtest is fixed
inline void PrintTo(const List &o, std::ostream *os) { *os << o.Str(); };

// The smallisp cons function
// Construct a cons cell containing object o1 and object o2
// If object o2 is a list, wrap the cons cell into a list
// To ensure correct dispatch. Before calling cons
// A runtime type check has to be done, and proper casting is required
const ConsC *Cons(const Object *o1, const Object *o2);
const List *Cons(const Object *o1, const List *o2);
inline bool IsNil(const Object *o) { return o == NIL; };
}  // namespace sl

#endif
