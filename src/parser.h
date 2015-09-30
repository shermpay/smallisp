/* -*- mode: C++ -*- */
#ifndef _PARSER_DEF
#define _PARSER_DEF
#include "list.h"

enum ParserErrorType {
  Parser_Error_No_Error,
  Parser_Error_No_Match,
};

struct ParserError {
  ParserErrorType Type;
  int Linum;
};

ParserError makeError(ParserErrorType Type, int Linum);

ParserError parseString(char *Input, List **ProgSexp);

#endif
