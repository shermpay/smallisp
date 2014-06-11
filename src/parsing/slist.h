#include <stdbool.h>
/* Struct representing a single Cons cell */
typedef struct cons_t {
    int val;
    struct cons_t *next;
} Cons;

/* Struct representing a Smallisp List (Singly linked-list) */
typedef struct slist_t {
    Cons *head;
    int length;
} Slist;

/* Creates a new cons cell with value x */
Cons *new_cons(int x);

/* Destructor for cons cells, deletes itself and returns a pointer to
   the next cons cell */
Cons *del_cons(Cons *c);

/* Creates a new slist */
Slist *new_slist();

/* Destructor for slist */
void del_slist(Slist *sl);

/* Adds a new Cons cell at the front of the list */
void slist_cons(Slist *sl, Cons *c);

/* Returns true if sl is empty */
bool slist_empty(Slist *sl);

/* Removes and returns the head of the list */
Cons *slist_pop(Slist *sl);

/* Prints slist in (a b c) format */
void print_slist(Slist *sl);
