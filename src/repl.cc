#include <cstdio>
#include <cstdlib>
#include <editline/readline.h>

#include "parser.h"

int StartRepl()
{
    puts("Smallisp REPL V0.9.");
    puts("Press Ctrl-C to exist\n");

    Object *Expr = static_cast<Object*>(malloc(sizeof(*Expr)));
    while(1) {
	char *Input = readline("slisp> ");
	add_history(Input);
        parseString(Input, &Expr);
        printf("Object: %s\n", objToStr(Expr));
	free(Input);
    }
    return 0;
}
