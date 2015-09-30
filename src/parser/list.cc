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
#include <cstdlib>
#include <cstdio>
#include "list.h"

/* Creates a new cons cell with value x */
Cons *new_cons(Object *x)
{
  Cons *res = static_cast<Cons*>(malloc(sizeof(Cons)));
  res->val = x;
  res->next = NULL;
  return res;
}

/* Destructor for cons cells, deletes itself and returns a pointer to
   the next cons cell */
Cons *del_cons(Cons *c)
{
  Cons *next = NULL;
  if (c->next) {
     next = c->next;
  }
  del_object(c->val);
  free(c);
  return next;
}

/* Creates a new list */
List *new_list()
{
  List *res = static_cast<List*>(malloc(sizeof(List)));
  res->head = NULL;
  res->length = 0;
  return res;
}

/* Destructor for list */
void del_list(List *sl)
{
  if (sl != NULL) {
    Cons *cur = sl->head;
    while (cur) {
      cur = del_cons(cur);
    }
    free(sl);
  }
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
int list_pop(List *sl, Cons **cons)
{
  /* Error if trying to pop empty list */
  if (list_empty(sl)) {
    fprintf(stderr, "Attempting to Pop empty list\n");
    return 1;
  }
    
  *cons = sl->head;
  sl->head = sl->head->next;
  sl->length--;
  return 0;
}

/* Prints list in (a b c) format */
void print_list(List *sl)
{
  Cons *cur = sl->head;
  char *str;
  putchar('(');
  if (!list_empty(sl)) {
    str = obj_tostr(cur->val);
    printf("%s", str);
    free(str);
    cur = cur->next;
    for (int i = 1; i < sl->length; i++) {
      str = obj_tostr(cur->val);
      printf(" %s", str);
      free(str);
    }
  }
  puts(")");
}
