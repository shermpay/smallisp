#include <stdio.h>
#include <stdlib.h>
#include "sltypes.h"

int main(int argc, char *argv[])
{
    SlType x; x.sl_num = 1;
    Object *obj = new_object(x, SL_NUM);
    
}
