/* -*- mode: C++ -*- */
#ifndef _LIST_DEF
#define _LIST_DEF

#include <cstdlib>

namespace sl {
struct Object;

struct Cons {
  Object &Obj;
  Cons &Next;
};

class List {
public:
  Cons *Head;
};

/* Creates a new cons cell with value x */
Cons *newCons(Object *x);

/* Destructor for cons cells, deletes itself and returns a pointer to
   the next cons cell */
Cons *delCons(Cons *c);

/* Creates a new list */
List *newList();

/* Destructor for list */
void delList(List *sl);

/* Adds a new Cons cell at the front of the list */
void listCons(List *sl, Cons *c);

/* Returns true if sl is empty */
bool listEmpty(const List *sl);

/* Removes and returns the head of the list in a return argument.
 Returns 0 for success and 1 for failure. */
int listPop(const List *sl, Cons **cons);

List *listTail(const List *sl);

size_t listCount(const List *sl);

/* Prints list in (a b c) format */
void printList(const List *sl);

} // namespace sl

#endif
