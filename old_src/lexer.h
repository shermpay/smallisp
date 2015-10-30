/* -*- mode: C++ -*-
  Author: Sherman Pay
  Version: 0.1; Thursday, 07. August 2014
  Lexical Analysis of input file.
  TokenStream is an object defined in token.h
 */
#ifndef _LEXER_DEF
#define _LEXER_DEF
#include <istream>
#include <cstdio>
#include "token.h"
TokenStream* lexer(std::istream &Stream);
#endif
