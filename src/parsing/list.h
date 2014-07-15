#include <stdbool.h>
#include "sltypes.h"

/* Creates a new cons cell with value x */
Cons *new_cons(Object *x);

/* Destructor for cons cells, deletes itself and returns a pointer to
   the next cons cell */
Cons *del_cons(Cons *c);

/* Creates a new list */
List *new_list();

/* Destructor for list */
void del_list(List *sl);

/* Adds a new Cons cell at the front of the list */
void list_cons(List *sl, Cons *c);

/* Returns true if sl is empty */
bool list_empty(List *sl);

/* Removes and returns the head of the list */
Cons *list_pop(List *sl);

/* Prints list in (a b c) format */
void print_list(List *sl);
