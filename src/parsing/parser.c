#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
	return 1;
    }

    FILE *input_file = fopen(argv[1], "r");

    TokenStream *stream = lexer(input_file);
    while (has_token(stream)) {
	Token *token = take_token(stream);
    }
}
