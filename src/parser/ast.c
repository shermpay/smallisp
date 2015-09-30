#include <stdlib.h>
#include <stdio.h>
#include "sltypes.h"
#include "list.h"
#include "ast.h"


Ast *new_tree()
{
    Ast *t = malloc(sizeof(Ast));
    t->root = NULL;
    return t;
}

void print_tree(Ast *tree)
{
    print_tree_aux(tree->root);
}

void print_tree_aux(List *root)
{
    if (root != NULL) {
	print_list(root);
    }
}
