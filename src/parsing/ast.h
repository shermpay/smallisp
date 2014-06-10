typedef struct node_t {
    struct node_t *left;
    struct node_t *right;
    int val;
} Node;

typedef struct {
    Node *root;
} Ast;

Node *new_node(int x);
Ast *new_tree();
void print_node(node_t *n);
void print_tree();
