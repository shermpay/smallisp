#include <stdio.h>
#include <stdlib.h>
#include "sltypes.h"

SlType new_sl_num(SlType *type, int x)
{
    
}

Object *new_object(SlType x, int type)
{
    Object *res = malloc(sizeof(Object));
    res->type = type;
    switch (type) {
	case SL_NUM:
	    res->val.sl_num = x.sl_num;
	    break;
	case SYMBOL:
	    res->val.symbol = x.symbol;
	    break;
	case SL_CHAR:
	    res->val.sl_char = x.sl_char;
	    break;
	case SL_BOOL:
	    res->val.sl_bool = x.sl_bool;
	    break;
	case LIST:
	    res->val.list = x.list;
	    break;
	default:
	    fprintf(stderr, "Invalid Type\n");
	    return 0;
    }
    return res;
}

void del_object(Object *x)
{
    
}
