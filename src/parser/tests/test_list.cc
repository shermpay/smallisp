#include <stdlib.h>
#include <stdio.h>
#include "../list.h"
#include "../sltypes.h"

int main(void)
{
  Object *zero = new_num(0);
  Object *one = new_num(1);
  Object *two = new_num(2);
  List *my_list = new_list();
  Cons *cons;
  list_cons(my_list, new_cons(zero));
  list_cons(my_list, new_cons(one));
  list_cons(my_list, new_cons(two));
  print_list(my_list);
  list_pop(my_list, &cons);
  del_cons(cons);
  print_list(my_list);
  list_pop(my_list, &cons);
  del_cons(cons);
  del_list(my_list);
  return 0;
}

