/*
  Sherman Pay Jing Hao
  Tuesday, 10. June 2014
  
  Smallisp List
  -------------
  Current Implementation:
  1. Singly Linked-list
  2. Build up of cons cell
  3. Bare minimal functions
  
  Future:
  1. Persistent Linked-list
 */
#include <stdlib.h>
#include <stdio.h>
#include "slist.h"

/* Creates a new cons cell with value x */
Cons *new_cons(int x)
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

/* Creates a new slist */
Slist *new_slist()
{
    Slist *res = malloc(sizeof(Slist));
    res->head = NULL;
    return res;
}

/* Destructor for slist */
void del_slist(Slist *sl)
{
    Cons *cur = sl->head;
    int c_count = sl->length;	/* Amount of cons cells */
    for (int i = 0; i < c_count; i++) {
	free(slist_pop(sl));
    }
    free(sl);
}

/* Adds a new Cons cell at the front of the list */
void slist_cons(Slist *sl, Cons *c)
{
    c->next = sl->head;
    sl->head = c;
    sl->length++;
}

/* Returns true if sl is empty */
bool slist_empty(Slist *sl)
{
    return sl->length == 0;
}

/* Removes and returns the head of the list */
Cons *slist_pop(Slist *sl)
{
    /* Error if trying to pop empty list */
    if (slist_empty(sl)) {
	fprintf(stderr, "Attempting to Pop empty slist\n");
	return 0;
    }
    
    Cons *res = sl->head;
    sl->head = sl->head->next;
    sl->length--;
    return res;
}

/* Prints slist in (a b c) format */
void print_slist(Slist *sl)
{
    Cons *cur = sl->head;
    putchar('(');
    if (!slist_empty(sl)) {
	printf("%d", cur->val);
	cur = cur->next;
	for (int i = 1; i < sl->length; i++) {
	    printf(" %d", cur->val);
	}
    }
    puts(")");
}
