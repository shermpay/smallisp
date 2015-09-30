#include <stdlib.h>
#include <stdio.h>
#include "../list.h"
#include "../sltypes.h"

int main(void)
{
  Object *zero = newNum(0);
  Object *one = newNum(1);
  Object *two = newNum(2);
  List *my_list = newList();
  Cons *cons;
  listCons(my_list, newCons(zero));
  listCons(my_list, newCons(one));
  listCons(my_list, newCons(two));
  printList(my_list);
  listPop(my_list, &cons);
  delCons(cons);
  printList(my_list);
  listPop(my_list, &cons);
  delCons(cons);
  delList(my_list);
  return 0;
}

