#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "sltypes.h"

SlVal *newNumVal(int X) {
  SlVal *Val = static_cast<SlVal *>(malloc(sizeof(SlVal)));
  Val->SlNum = X;
  return Val;
}

SlVal *newSymbolVal(const char *X) {
  SlVal *Val = static_cast<SlVal *>(malloc(sizeof(SlVal)));
  Val->Symbol = Symbol::get(std::string(X));
  return Val;
}

SlVal *newCharVal(char X) {
  SlVal *Val = static_cast<SlVal *>(malloc(sizeof(SlVal)));
  Val->SlChar = X;
  return Val;
}

SlVal *newBoolVal(int X) {
  SlVal *val = static_cast<SlVal *>(malloc(sizeof(SlVal)));
  val->SlBool = X;
  return val;
}

SlVal *newStringVal(const char *X) {
  SlVal *val = static_cast<SlVal *>(malloc(sizeof(SlVal)));
  val->SlString = X;
  return val;
}

Object *newNum(int X) { return newObject(newNumVal(X), SlNumTy); }

Object *newSymbol(const char *X) {
  return newObject(newSymbolVal(X), SymbolTy);
}

Object *newChar(char X) { return newObject(newCharVal(X), SlCharTy); }

Object *newBool(int X) { return newObject(newBoolVal(X), SlBoolTy); }

Object *newString(const char *X) {
  return newObject(newStringVal(X), SlStringTy);
}

Object *listToObject(struct list_t *Lst) {
  SlVal *Val = static_cast<SlVal *>(malloc(sizeof(SlVal)));
  Val->List = Lst;
  return newObject(Val, ListTy);
}

Object *newSexp(struct list_t *Lst) {
  SlVal *Val = static_cast<SlVal *>(malloc(sizeof(SlVal)));
  Val->List = Lst;
  return newObject(Val, SexpTy);
}

Object *newObject(SlVal *X, SlType Type) {
  Object *Res = static_cast<Object *>(malloc(sizeof(Object)));
  Res->Type = Type;
  Res->Val = X;
  return Res;
}

void delObject(Object *X) {
  free(X->Val);
  free(X);
}

std::string objToStr(const Object *X) {
  /* TODO: Implement this. */
  int Size, Len;
  Size = 64;
  char *Buff = static_cast<char *>(malloc(sizeof(char) * Size));
  switch (X->Type) {
  case SlNumTy:
    Len = snprintf(Buff, Size, "%ld", X->Val->SlNum);
    if (Len >= Size) {
      Buff = (char *)realloc(Buff, sizeof(char) * Len);
      snprintf(Buff, Size, "%ld", X->Val->SlNum);
    }
    return std::string(Buff);
  case SymbolTy:
    Len = X->Val->Symbol->name().size();
    if (Len >= Size) {
      Buff = (char *)realloc(Buff, sizeof(char) * Len);
    }
    memcpy(Buff, X->Val->Symbol->name().c_str(), sizeof(char) * Len);
    return std::string(Buff);
  // return buff;
  case SlCharTy:
    return std::string("CHAR");
  case SlBoolTy:
    return std::string("BOOL");
  case SlStringTy:
    Len = X->Val->Symbol->name().size();
    if (Len >= Size) {
      Buff = (char *)realloc(Buff, sizeof(char) * Len);
    }
    memcpy(Buff, X->Val->SlString, sizeof(char) * Len);
    return std::string(Buff);
  case ListTy:
  case SexpTy:
    return listToStr(X->Val->List);
  }
}
