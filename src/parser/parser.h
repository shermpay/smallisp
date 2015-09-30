#ifndef _PARSER_DEF
#define _PARSER_DEF

enum  ParserErrorType {
  Parser_Error_No_Error,
  Parser_Error_No_Match,
};

struct ParserError {
  ParserErrorType Type;
  int Linum;
};

ParserError makeError(ParserErrorType Type, int Linum);

#endif
