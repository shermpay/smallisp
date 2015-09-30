#include <cstdlib>
#include <cstdio>

#include <stack>

#include "lexer.h"
#include "parser.h"
#include "list.h"

ParserError parse_tokens(TokenStream *stream, List **prog) 
{
  std::stack<Token*> delim_stack;
  *prog = new_list();
  while (has_token(stream)) {
    Token *token = take_token(stream);
    // switch (token->type) {
    //   case OPEN_PAREN:
    //   case OPEN_BRACK:
    //     delim_stack.push(token)
    // }
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

