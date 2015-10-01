/* -*- mode: C++ -*- */
#ifndef _PARSER_DEF
#define _PARSER_DEF
#include "list.h"
#include "token.h"

enum ParserErrorType {
  Parser_Error_No_Error,
  Parser_Error_No_Match,
};

struct ParserError {
  ParserErrorType Type;
  int Linum;
};

const ParserError ParserNoError = {
  Parser_Error_No_Error,
  0,
};

ParserError makeError(ParserErrorType Type, int Linum);

ParserError parseExpr(TokenStream *Stream, Object **Expr);
ParserError parseSexp(TokenStream *Stream, List **NewSexp);
ParserError parseString(char *Input, Object **Expr);

#endif
