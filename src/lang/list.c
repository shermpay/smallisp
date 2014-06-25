/*
  Sherman Pay Jing Hao
  Tuesday, 10. June 2014
  
  Smallisp List
  -------------
  Current Implementation:
  1. Singly Linked-list
  2. Build up of cons cell
  3. Bare minimal functions
  4. Cons cell of SlObj
  
  Todo:
  1. Use dynamic types

  Future:
  1. Garbage Collection
  2. Persistent Linked-list
 */
#include <stdlib.h>
#include <stdio.h>
#include "list.h"

/* Creates a new cons cell with value x */
Cons *new_cons(Object *x)
{
    Cons *res = malloc(sizeof(Cons));
    res->val = x;
    res->next = NULL;
    return res;
}

/* Destructor for cons cells, deletes itself and returns a pointer to
   the next cons cell */
Cons *del_cons(Cons *c)
{
    Cons *next = c->next;
    free(c);
    return next;
}

/* Creates a new list */
List *new_list()
{
    List *res = malloc(sizeof(List));
    res->head = NULL;
    return res;
}

/* Destructor for list */
void del_list(List *sl)
{
    int c_count = sl->length;	/* Amount of cons cells */
    for (int i = 0; i < c_count; i++) {
	free(list_pop(sl));
    }
    free(sl);
}

/* Adds a new Cons cell at the front of the list */
void list_cons(List *sl, Cons *c)
{
    c->next = sl->head;
    sl->head = c;
    sl->length++;
}

/* Returns true if sl is empty */
bool list_empty(List *sl)
{
    return sl->length == 0;
}

/* Removes and returns the head of the list */
Cons *list_pop(List *sl)
{
    /* Error if trying to pop empty list */
    if (list_empty(sl)) {
	fprintf(stderr, "Attempting to Pop empty list\n");
	return 0;
    }
    
    Cons *res = sl->head;
    sl->head = sl->head->next;
    sl->length--;
    return res;
}
