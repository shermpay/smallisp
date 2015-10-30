/*
  Author: Sherman Pay
  Version: 0.1; Thursday, 07. August 2014
  TokenStreams are implemented as a Doubly Linked List with a small interface.
  Memory usage is not optimized for string outputs.
 */
#include "token.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <string>
#include <sstream>

std::string tokenToStr(const Token *Token) {
  std::ostringstream SStream;
  switch (Token->Type) {
  case TOK_OpenParen:
    SStream << "(TOK_OpenParen, ln: " << Token->Linum << ")";
    break;
  case TOK_CloseParen:
    SStream << "(TOK_CloseParen, ln: " << Token->Linum << ")";
    break;
  case TOK_OpenBrack:
    SStream << "(TOK_OpenBrack, ln: " << Token->Linum << ")";
    break;
  case TOK_CloseBrack:
    SStream << "(TOK_CloseBrack, ln: " << Token->Linum << ")";
    break;
  case TOK_Symbol:
    SStream << "(TOK_Symbol: " << Token->Val.TokStr << ", ln: " << Token->Linum
            << ")";
    break;
  case TOK_Number:
    SStream << "(TOK_Number: " << Token->Val.TokNum << ", ln: " << Token->Linum
            << ")";
    break;
  case TOK_String:
    SStream << "(TOK_String: " << Token->Val.TokStr << ", ln: " << Token->Linum
            << ")";
    break;
  default:
    SStream << "Unexpected token on line: " << Token->Linum;
  }
  return SStream.str();
}

TokenStream *newTokenStream(void) {
  TokenStream *Stream = (TokenStream *)malloc(sizeof(TokenStream));
  Stream->Front = NULL;
  Stream->Back = NULL;
  return Stream;
}

void flushStream(TokenStream *Stream) {

  StreamNode *Old;
  StreamNode *Curr = Stream->Front;
  while (Curr != NULL) {
    Old = Curr;
    Curr = Curr->Next;
    Old->Next = NULL;
    Old->Prev = NULL;
    free(Old);
  }
  free(Curr);
  free(Stream);
}

void pushToken(TokenStream *Stream, Token *Token) {
  StreamNode *Node = (StreamNode *)malloc(sizeof(StreamNode));
  Node->Token = Token;
  Node->Prev = Stream->Back;
  Node->Next = NULL;
  if (Stream->Back != NULL) {
    Stream->Back->Next = Node;
  } else {
    Stream->Front = Node;
  }
  Stream->Back = Node;
}

const Token *takeToken(TokenStream *Stream) {
  StreamNode *Node = Stream->Front;
  Stream->Front = Stream->Front->Next;
  Node->Next = NULL;
  Node->Prev = NULL;
  Token *Result = Node->Token;
  free(Node);
  return Result;
}

const Token *peekToken(TokenStream *Stream) {
  StreamNode *Node = Stream->Front;
  Token *Result = Node->Token;
  return Result;
}

void retToken(TokenStream *Stream, Token *Token) {
  StreamNode *Node = (StreamNode *)malloc(sizeof(StreamNode));
  Node->Token = Token;
  Node->Next = Stream->Front;
  Node->Prev = NULL;
  Stream->Front = Node;
  if (Stream->Back == NULL) {
    Stream->Back = Node;
  }
}

bool hasToken(TokenStream *Stream) { return Stream->Front != NULL; }

std::string streamTostr(TokenStream *Stream) {
  std::ostringstream SStream;
  SStream << '[';
  while (hasToken(Stream)) {
    const Token *Tok = takeToken(Stream);
    std::string Str = tokenToStr(Tok);
    SStream << Str;
  }
  SStream << ']';
  return SStream.str();
}
