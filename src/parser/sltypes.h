/* 
   Sherman Pay Jing Hao
   Friday, 13. June 2014
   Smallisp type definitions
*/

#ifndef _SLTYPES_DEF
#define _SLTYPES_DEF

#define MAX_STR_LEN 512
#define TRUE_STR "true"
#define FALSE_STR "false"

typedef enum sltype_t {SL_NUM, SYMBOL, SL_CHAR, SL_BOOL, SL_STRING, LIST} SlType;

/* Struct representing a single Cons cell */
typedef struct cons_t {
    struct object_t *val;
    struct cons_t *next;
} Cons;

/* Struct representing a Smallisp List (Singly linked-list) */
typedef struct list_t {
    Cons *head;
    int length;
} List;

/* Union representing all primitive Smallisp types */
typedef union slval_t {
    long sl_num;		
    char *symbol;
    char sl_char;	
    int sl_bool;	
    char *sl_string;
    List list;
} SlVal;

typedef struct object_t {
    SlVal *val;
    SlType type;
} Object;

Object *new_num(int x);
Object *new_symbol(char *x);
Object *new_char(char x);
Object *new_bool(int x);
Object *new_string(char *x);
Object *new_object(SlVal *x, SlType type);
char *obj_tostr(Object *);
void del_object(Object *x);

#endif
