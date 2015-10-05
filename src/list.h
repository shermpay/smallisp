/* -*- mode: C++ -*- */
#ifndef _LIST_DEF
#define _LIST_DEF

#include "sltypes.h"

typedef struct cons_t {
  Object *Val;
  struct cons_t *Next;
} Cons;

/* Struct representing a Smallisp List (Singly linked-list) */
typedef struct list_t {
  Cons *Head;
} List;

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

List *listTail(List *sl);

/* Prints list in (a b c) format */
void printList(const List *sl);


#endif
