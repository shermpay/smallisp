#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "list.h"

ParserError parse_tokens(TokenStream *stream, List **prog) 
{
  *prog = new_list();
  while (has_token(stream)) {
    Token *token = take_token(stream);
  }
  return NO_ERROR;
}

int main(int argc, char *argv[])
{
  if (argc < 2) {
    puts("Parser requires FILE argument");
    return 1;
  }

  FILE *input_file = fopen(argv[1], "r");

  List *prog;
  TokenStream *stream = lexer(input_file);
  parse_tokens(stream, &prog);
}

