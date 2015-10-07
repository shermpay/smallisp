/* -*- mode: C++ -*-
   Sherman Pay Jing Hao
   Friday, 13. June 2014
   Smallisp type definitions
*/

#ifndef _SLTYPES_DEF
#define _SLTYPES_DEF

#include "symbol.h"
#include "list.h"
#include <string>

#define MAX_STR_LEN 512
#define TRUE_STR "true"
#define FALSE_STR "false"

typedef enum sltype_t {
  SlNumTy,
  SymbolTy,
  SlCharTy,
  SlBoolTy,
  SlStringTy,
  ListTy,
  SexpTy
} SlType;

struct Object;

class SlFunction {
public:
  SlFunction(const int &NumArgs) : NumArgs(NumArgs) {}
  int numArgs() { return this->NumArgs; };

private:
  const int NumArgs;
};

/* Union representing all primitive Smallisp types */
typedef union slval_t {
  long SlNum;
  Symbol *Symbol;
  char SlChar;
  // TODO: Change to bool.
  int SlBool;
  const char *SlString;
  struct list_t *List;
  SlFunction *SlFunction;
} SlVal;

struct Object {
  SlVal *Val;
  SlType Type;
};

Object *newNum(int x);
Object *newSymbol(const char *x);
Object *newChar(char x);
Object *newBool(int x);
Object *newString(const char *x);
Object *newObject(SlVal *x, SlType type);
Object *listToObject(struct list_t *Lst);
Object *newSexp(struct list_t *Lst);
std::string objToStr(const Object *);
std::string listToStr(const struct list_t *Sl);
void delObject(Object *x);

#endif
