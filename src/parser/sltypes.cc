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

Object *new_num(int x)
{
  return new_object(new_num_val(x), SL_NUM);
}

Object *new_symbol(char *x)
{
  return new_object(new_symbol_val(x), SYMBOL);
}

Object *new_char(char x) 
{
  return new_object(new_char_val(x), SL_CHAR);
}

Object *new_bool(int x)
{
  return new_object(new_bool_val(x), SL_BOOL);
}

Object *new_string(char *x)
{
  return new_object(new_string_val(x), SL_STRING);
}

Object *new_object(SlVal *x, SlType type)
{
  Object *res = static_cast<Object*>(malloc(sizeof(Object)));
  res->type = type;
  res->val = x;
  return res;
}

void del_object(Object *x)
{
  free(x->val);
  free(x);
}

char *obj_tostr(Object *x)
{
  /* TODO: Implement this. */
  int size, len;
  size = 64;
  char *buff = static_cast<char*>(malloc(sizeof(char) * size));
  switch (x->type) {
    case SL_NUM:
      len = snprintf(buff, size, "%ld", x->val->sl_num);
      if (len >= size) {
        buff = (char*)realloc(buff, sizeof(char) * len);
        snprintf(buff, size, "%ld", x->val->sl_num);
      }
      return buff;
    case SYMBOL:
      return "SYMBOL";
    case SL_CHAR:
      return "CHAR";
    case SL_BOOL:
      return "BOOL";
    case SL_STRING:
      return "STRING";
    case LIST:
      return "LIST";
  }
}
