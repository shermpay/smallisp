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
#include "list.h"

#include <cstdlib>
#include <cstdio>
#include <cstring>

/* Creates a new cons cell with value x */
Cons *newCons(Object *X) {
  Cons *Res = static_cast<Cons *>(malloc(sizeof(Cons)));
  Res->Val = X;
  Res->Next = NULL;
  return Res;
}

/* Destructor for cons cells, deletes itself and returns a pointer to
   the next cons cell */
Cons *delCons(Cons *C) {
  Cons *Next = NULL;
  if (C->Next) {
    Next = C->Next;
  }
  delObject(C->Val);
  free(C);
  return Next;
}

/* Creates a new list */
List *newList() {
  List *Res = static_cast<List *>(malloc(sizeof(List)));
  Res->Head = NULL;
  return Res;
}

/* Destructor for list */
void delList(List *Sl) {
  if (Sl != NULL) {
    Cons *Cur = Sl->Head;
    while (Cur) {
      Cur = delCons(Cur);
    }
    free(Sl);
  }
}

/* Adds a new Cons cell at the front of the list */
void listCons(List *Sl, Cons *C) {
  C->Next = Sl->Head;
  Sl->Head = C;
}

/* Returns true if sl is empty */
bool listEmpty(const List *Sl) { return Sl->Head == NULL; }

/* Removes and returns the head of the list */
int listPop(List *Sl, Cons **Cons) {
  /* Error if trying to pop empty list */
  if (listEmpty(Sl)) {
    fprintf(stderr, "Attempting to Pop empty list\n");
    return 1;
  }

  *Cons = Sl->Head;
  Sl->Head = Sl->Head->Next;
  return 0;
}

List *listTail(const List *Sl) {
  List *Tail = newList();
  Tail->Head = Sl->Head->Next;
  return Tail;
}

/* Prints list in (a b c) format */
void printList(const List *Sl) {
  Cons *Cur = Sl->Head;
  std::string Str;
  putchar('(');
  if (!listEmpty(Sl)) {
    Str = objToStr(Cur->Val);
    printf("%s", Str.c_str());
    while (Cur->Next != NULL) {
      Cur = Cur->Next;
      Str = objToStr(Cur->Val);
      printf(" %s", Str.c_str());
    }
  }
  puts(")");
}

// TODO: Fix this. (Use strncat)
std::string listToStr(const List *Sl) {
  Cons *Cur = Sl->Head;
  std::string Buff;
  Buff.push_back('(');
  if (!listEmpty(Sl) && Cur) {
    Buff += (objToStr(Cur->Val));
    while (Cur && Cur->Next) {
      Cur = Cur->Next;
      Buff.push_back(' ');
      Buff += objToStr(Cur->Val);
    }
  }
  Buff.push_back(')');
  return Buff;
}
