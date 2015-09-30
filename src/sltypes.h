/* -*- mode: C++ -*-
   Sherman Pay Jing Hao
   Friday, 13. June 2014
   Smallisp type definitions
*/

#ifndef _SLTYPES_DEF
#define _SLTYPES_DEF

#define MAX_STR_LEN 512
#define TRUE_STR "true"
#define FALSE_STR "false"

typedef enum sltype_t {SlNumTy, SymbolTy, SlCharTy, SlBoolTy, 
                       SlStringTy, ListTy, SexpTy} SlType;

/* Struct representing a single Cons cell */
struct cons_t;
struct list_t;

/* Union representing all primitive Smallisp types */
typedef union slval_t {
  long sl_num;		
  char *symbol;
  char sl_char;	
  // TODO: Change to bool.
  int sl_bool;	
  char *sl_string;
  struct list_t *list;
} SlVal;

typedef struct object_t {
  SlVal *val;
  SlType type;
} Object;

Object *newNum(int x);
Object *newSymbol(char *x);
Object *newChar(char x);
Object *newBool(int x);
Object *newString(char *x);
Object *newObject(SlVal *x, SlType type);
Object *listToObject(struct list_t *Lst);
Object *newSexp(struct list_t *Lst);
char *objToStr(Object *);
char *listToStr(struct list_t *Sl);
void delObject(Object *x);

#endif
