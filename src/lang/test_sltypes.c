#include <stdio.h>
#include <stdlib.h>
#include "sltypes.h"

int main(int argc, char *argv[])
{
    Object *num = new_object(new_num(1), SL_NUM);
    Object *sym = new_object(new_symbol("foo"), SYMBOL);
    del_object(num);
    del_object(sym);
}
