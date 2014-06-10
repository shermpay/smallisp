#include <stdlib.h>
#include <stdio.h>

typedef struct cons_t {
    int val;
    struct cons_t next;
} Cons;

typedef struct slist_t {
    Cons *start;
    Const *end;
} Slist;

Cons *new_cons(int x)
{
    Cons *res = malloc(sizeof(Cons));
    res->val = x;
    res->next = NULL;
    return res;
}

Slist *new_slist()
{
    Slist *res = malloc(sizeof(Slist));
    res->start = new_cons(0);
    res->end = res->start;
    return res;
}
