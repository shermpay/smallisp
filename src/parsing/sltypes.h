/* Sherman Pay Jing Hao
   Friday, 13. June 2014
Smallisp type definitions*/

enum SlTypes {SL_NUM, SYMBOL, SL_CHAR, SL_BOOL, LIST};

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
    int sl_num;		
    char *symbol;
    char sl_char;	
    int sl_bool;	
    List list;
} SlType;

typedef struct object_t {
    SlType val;
    int type;
} Object;

Object *new_object(SlType x, int type);
void del_objet(Object *x);
