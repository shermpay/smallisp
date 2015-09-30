#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <editline/readline.h>

#include "parser/lexer.h"

int StartRepl()
{
    puts("Smallisp REPL V0.9.");
    puts("Press Ctrl-C to exist\n");
    TokenStream *TokStream;

    while(1) {
	char *input = readline("slisp> ");
	add_history(input);
        std::istringstream Stream(input);
        TokStream = lexer(Stream);
	free(input);
    }
    return 0;
}
