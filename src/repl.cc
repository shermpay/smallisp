#include <cstdio>
#include <cstdlib>
#include <editline/readline.h>

#include "parser.h"

int StartRepl()
{
    puts("Smallisp REPL V0.9.");
    puts("Press Ctrl-C to exist\n");

    List *Prog = static_cast<List*>(malloc(sizeof(*Prog)));
    while(1) {
	char *Input = readline("slisp> ");
	add_history(Input);
        parseString(Input, &Prog);
        printf("Sexp: %s\n", listToStr(Prog));
	free(Input);
    }
    return 0;
}
