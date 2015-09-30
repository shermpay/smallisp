#include <cstdlib>
#include <cstdio>

#include <stack>

#include "lexer.h"
#include "parser.h"
#include "list.h"

ParserError makeError(ParserErrorType Type, int Linum) {
  return {Type, Linum};
}

ParserError parseTokens(TokenStream *Stream, List **Prog) {
  std::stack<Token*> delim_stack;
  *Prog = new_list();
  while (has_token(Stream)) {
    Token *Token = take_token(Stream);
    switch (Token->type) {
      case OPEN_PAREN:
      case OPEN_BRACK:
        delim_stack.push(Token);
        break;
      case CLOSE_PAREN:
        Token = delim_stack.top();
        if (Token->type != OPEN_PAREN) {
          makeError(Parser_Error_No_Match, Token->linum);
        } 
        break;
    }
  }
  return makeError(Parser_Error_No_Error, 0);
}

