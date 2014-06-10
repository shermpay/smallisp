#include <stdlib.h>
#include <stdio.h>
#include "types.h"
#include "ast.h"


Node *new_node(int x)
{
    Node *n = malloc(sizeof(Node));
    n->val = x;
    n->left = NULL;
    n->right = NULL;
    return n;
}

Ast *new_tree()
{
    Ast *t = malloc(sizeof(Ast));
    t->root = new_node(0);
    return t;
}

void print_node(Node *n)
{
    if (n == NULL) {
	fprintf(stderr, "Encountered NULL node\n");
	return;
    }
    printf("%d\n", n->val);
}

void print_tree()
{
    return;
}
