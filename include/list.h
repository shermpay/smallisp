/* -*- mode: C++ -*- */
#ifndef _LIST_DEF
#define _LIST_DEF

#include "Objects.h"
#include <cstdlib>

namespace sl {

struct Cons {
  Object *Obj;
  Cons *Next;
};

/* Creates a new cons cell with value x */
Cons *newCons(Object *x);

/* Destructor for cons cells, deletes itself and returns a pointer to
   the next cons cell */
Cons *delCons(Cons *c);

// The standard Lisp consing function.
Cons *cons(Object *o1, Object *o2);

class List : Object {
public:
  Cons *Head;
  /* Creates a new list */
  List();

  /* Destructor for list */
  ~List();

  /* Returns true if sl is empty */
  bool empty();

  /* Push a new Object to the front of the list */
  void push(Object *obj);

  /* Removes and returns the head of the list in a return argument.
     Returns 0 for success and 1 for failure. */
  int pop(Cons **cons);

  List *tail();

  size_t count();

  /* Prints list in (a b c) format */
  void print();
};

} // namespace sl

#endif
