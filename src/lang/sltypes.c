#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sltypes.h"

SlType *new_num(int x)
{
    SlType *type = malloc(sizeof(SlType));
    type->sl_num = x;
    return type;
}

SlType *new_symbol(char *x)
{
    SlType *type = malloc(sizeof(SlType));
    type->symbol = x;
    return type;
}

SlType *new_char(char x)
{
    SlType *type = malloc(sizeof(SlType));
    type->sl_char = x;
    return type;
}

SlType *new_bool(int x)
{
    SlType *type = malloc(sizeof(SlType));
    type->sl_bool = x;
    return type;
}

Object *new_object(SlType *x, int type)
{
    Object *res = malloc(sizeof(Object));
    res->type = type;
    res->val = x;
    return res;
}

void del_object(Object *x)
{
    free(x->val);
    free(x);
}
