/* 
   Sherman Pay Jing Hao
   Friday, 13. June 2014
   Smallisp type definitions
*/

#define MAX_STR_LEN 512
#define TRUE_STR "true"
#define FALSE_STR "false"

enum SlTypes {SL_NUM, SYMBOL, SL_CHAR, SL_BOOL, SL_STRING, LIST};

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

/* Union representing all Smallisp types */
typedef union sltype_t {
    long sl_num;		
    char *symbol;
    char sl_char;	
    int sl_bool;	
    char *sl_string;
    List list;
} SlType;

typedef struct object_t {
    SlType *val;
    int type;
} Object;

SlType *new_num(int x);
SlType *new_symbol(char *x);
SlType *new_char(char x);
SlType *new_bool(int x);
SlType *new_string(char *x);
Object *new_object(SlType *x, int type);
void del_object(Object *x);
