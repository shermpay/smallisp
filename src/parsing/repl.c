#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>

#include "lexer.h"

int main(int argc, char *argv[])
{
    puts("Smallisp REPL V0.9.");
    puts("Press Ctrl-C to exist\n");

    while(1) {
	char *input = readline("slisp> ");
	add_history(input);
	
	free(input);
    }
    return 0;
}
