/*
  Sherman Pay Jing Hao
  2014-06-24
  Core functions
*/
#include <stdio.h>
#include <stdlib.h>
#include "sltypes.h"
#include "list.h"

char *c_to_string(Object *x)
{

    switch (x->type) {
	case SL_NUM:
	    char buff[MAX_STR_LEN];
	    sprintf(buff, "%ld", x->val);
	    return buff;
	case SL_CHAR:
	    char buff[1] = { x->val }
	    return buff;
	case SL_BOOL:
	    return x->val ? TRUE_STR : FALSE_STR;
	case SL_STRING:
	    return x;
	case SYMBOL:
	    char *res = "#";
	    strcat(res, x->val);
	    return res
	case LIST:
	    /* Poor implementation */
	    char buff[MAX_STR_LEN];
	    buff[0] = '(';
	    Cons *cur = x->val;
	    for (int i = 0; i < x->val->length; i++) {
		strcat(buff, to_str(cur->val));
		cur = cur->next;
	    }
	    buff[x->val->length] = ')';
	    buff[x->val->length + 1] = '\0';
	    return buff;
    }
}

Object *to_string(Object *x)
{
    return new_object(new_string(c_to_string(x)), SL_STRING);
}
