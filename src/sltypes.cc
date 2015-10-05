#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "sltypes.h"

SlVal *newNumVal(int X)
{
  SlVal *Val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  Val->SlNum = X;
  return Val;
}

SlVal *newSymbolVal(const char *X)
{
  SlVal *Val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  Val->Symbol = new Symbol();
  Val->Symbol->Name = X;
  return Val;
}

SlVal *newCharVal(char X)
{
  SlVal *Val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  Val->SlChar = X;
  return Val;
}

SlVal *newBoolVal(int X)
{
  SlVal *val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  val->SlBool = X;
  return val;
}

SlVal *newStringVal(const char *X)
{
  SlVal *val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  val->SlString = X;
  return val;
}

Object *newNum(int X)
{
  return newObject(newNumVal(X), SlNumTy);
}

Object *newSymbol(const char *X)
{
  return newObject(newSymbolVal(X), SymbolTy);
}

Object *newChar(char X) 
{
  return newObject(newCharVal(X), SlCharTy);
}

Object *newBool(int X)
{
  return newObject(newBoolVal(X), SlBoolTy);
}

Object *newString(const char *X)
{
  return newObject(newStringVal(X), SlStringTy);
}

Object *listToObject(struct list_t *Lst) {
  SlVal *Val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  Val->List = Lst;
  return newObject(Val, ListTy);
}

Object *newSexp(struct list_t *Lst) {
  SlVal *Val = static_cast<SlVal*>(malloc(sizeof(SlVal)));
  Val->List = Lst;
  return newObject(Val, SexpTy);
}

Object *newObject(SlVal *X, SlType Type)
{
  Object *Res = static_cast<Object*>(malloc(sizeof(Object)));
  Res->Type = Type;
  Res->Val = X;
  return Res;
}

void delObject(Object *X)
{
  free(X->Val);
  free(X);
}

char *objToStr(const Object *X)
{
  /* TODO: Implement this. */
  int size, len;
  size = 64;
  char *buff = static_cast<char*>(malloc(sizeof(char) * size));
  switch (X->Type) {
    case SlNumTy:
      len = snprintf(buff, size, "%ld", X->Val->SlNum);
      if (len >= size) {
        buff = (char*)realloc(buff, sizeof(char) * len);
        snprintf(buff, size, "%ld", X->Val->SlNum);
      }
      return buff;
    case SymbolTy:
      memcpy(buff, X->Val->Symbol->Name, sizeof(char) * size);
      return buff;
    case SlCharTy:
      return "CHAR";
    case SlBoolTy:
      return "BOOL";
    case SlStringTy:
      memcpy(buff, X->Val->SlString, sizeof(char) * size);
      return buff;
    case ListTy:
    case SexpTy:
      return listToStr(X->Val->List);
  }
}
