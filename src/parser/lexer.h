/* -*- mode: C++ -*-
  Author: Sherman Pay
  Version: 0.1; Thursday, 07. August 2014
  Lexical Analysis of input file.
  TokenStream is an object defined in token.h
 */
#include <istream>
#include <cstdio>
#include "token.h"
TokenStream* lexer(std::istream &Stream);