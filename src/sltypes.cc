#include <cstdio>
#include <cstdlib>
#include "sltypes.h"

SlVal *new_num_val(int x)
{
  SlVal *val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  val->sl_num = x;
  return val;
}

SlVal *new_symbol_val(char *x)
{
  SlVal *val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  val->symbol = x;
  return val;
}

SlVal *new_char_val(char x)
{
  SlVal *val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  val->sl_char = x;
  return val;
}

SlVal *new_bool_val(int x)
{
  SlVal *val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  val->sl_bool = x;
  return val;
}

SlVal *new_string_val(char *x)
{
  SlVal *val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  val->sl_string = x;
  return val;
}

Object *newNum(int x)
{
  return newObject(new_num_val(x), SlNumTy);
}

Object *newSymbol(char *x)
{
  return newObject(new_symbol_val(x), SymbolTy);
}

Object *newChar(char x) 
{
  return newObject(new_char_val(x), SlCharTy);
}

Object *newBool(int x)
{
  return newObject(new_bool_val(x), SlBoolTy);
}

Object *newString(char *x)
{
  return newObject(new_string_val(x), SlStringTy);
}

Object *listToObject(struct list_t *Lst) {
  SlVal *Val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  Val->list = Lst;
  return newObject(Val, ListTy);
}

Object *newSexp(struct list_t *Lst) {
  SlVal *Val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  Val->list = Lst;
  return newObject(Val, SexpTy);
}

Object *newObject(SlVal *x, SlType type)
{
  Object *res = static_cast<Object*>(malloc(sizeof(Object)));
  res->type = type;
  res->val = x;
  return res;
}

void delObject(Object *x)
{
  free(x->val);
  free(x);
}

char *objToStr(Object *x)
{
  /* TODO: Implement this. */
  int size, len;
  size = 64;
  char *buff = static_cast<char*>(malloc(sizeof(char) * size));
  switch (x->type) {
    case SlNumTy:
      len = snprintf(buff, size, "%ld", x->val->sl_num);
      if (len >= size) {
        buff = (char*)realloc(buff, sizeof(char) * len);
        snprintf(buff, size, "%ld", x->val->sl_num);
      }
      return buff;
    case SymbolTy:
      return x->val->symbol;
    case SlCharTy:
      return "CHAR";
    case SlBoolTy:
      return "BOOL";
    case SlStringTy:
      return x->val->sl_string;
    case ListTy:
    case SexpTy:
      return listToStr(x->val->list);
  }
}
