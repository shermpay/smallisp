#include "sltypes.h"

typedef struct {
    List *root;
} Ast;

Ast *new_tree();
void print_tree(Ast *tree);
