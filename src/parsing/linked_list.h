typedef struct list_node ListNode;
struct list_node {
    void* item;
    ListNode *next;
    ListNode *prev;
};

typedef struct {
    ListNode *front;
    ListNode *back;
} LinkedList;

LinkedList *new_linkedlist(void);
void llist_pushh(LinkedList *list, void* item);
void llist_pusht(LinkedList *list, void* item);
void *llist_head(LinkedList *list);
void *llist_tail(LinkedList *list);
LinkedList *llist_rest(LinkedList *list);
void *llist_poph(LinkedList *list);
void *lllist_popt(LinkedList *list);
