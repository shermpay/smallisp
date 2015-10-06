/* -*- mode: C++ -*-
  Author: Sherman Pay
  Version: 0.1; Thursday, 07. August 2014
  Token and TokenStream definitions here.
 */
#ifndef _TOKEN_DEF
#define _TOKEN_DEF

#include <string>

enum TokenType {
  TOK_OpenParen,
  TOK_CloseParen,
  TOK_OpenBrack,
  TOK_CloseBrack,
  TOK_Number,
  TOK_String,
  TOK_Symbol,
  TOK_Invalid
};

union TokenVal {
  long TokNum;
  char *TokStr;
};

struct Token {
  int Linum; /* Aid error reporting */
  enum TokenType Type;
  union TokenVal Val;
};

char *tokenToStr(Token *);

/*
   Token Stream
   ------------
   Provides a simple API to interact with tokens via a FIFO Stream.
   You build a stream by pushing tokens onto the stream.

   And you can manipulate the tokens via the following functions:
   - take_token(TokenStream*) -> Token* : Take the next token from the stream
   - has_token(TokenStream*) -> bool : Check if the stream has any tokens
   - ret_token(TokenStream*) -> void : Return the last token you took from the
   stream
   ret_token technically allows you to insert anything to the front of the token
   stream.

   Finally you construct a stream with new_tokenstream(void), and flush/close
   the stream,
   with flush_stream(TokenStream*).
*/
struct StreamNode {
  Token *Token;
  StreamNode *Next;
  StreamNode *Prev;
};

struct TokenStream {
  StreamNode *Front;
  StreamNode *Back;
};
std::string tokenToStr(const Token *);
TokenStream *newTokenStream(void);
void flushStream(TokenStream *);
void pushToken(TokenStream *, Token *);
const Token *takeToken(TokenStream *);
const Token *peekToken(TokenStream *);
void retToken(TokenStream *, Token *);
bool hasToken(TokenStream *);
std::string streamToStr(const TokenStream *);

#endif
