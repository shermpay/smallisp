#include <stdio.h>
#include <stdlib.h>
#include "../sltypes.h"

int main(void)
{
    Object *num = new_num(1);
    Object *sym = new_symbol("foo");
    del_object(num);
    del_object(sym);
}
