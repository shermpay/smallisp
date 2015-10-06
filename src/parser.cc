#include <sstream>
#include <cstdlib>
#include <cstdio>

#include <list>

#include "lexer.h"
#include "token.h"
#include "parser.h"
#include "sltypes.h"
#include "list.h"

/* Grammar Definition
   Program -> Expr*
   Expr -> Sexpr | QList
   Sexpr -> ( "(" Lambda | Def | IfExpr | FunCall ")" )
   Lambda ->  "lambda" "(" Formal* ")" Expr
   Def -> "def" Ident Expr
   IfExpr -> "if" Expr Expr
   FunCall -> Ident+
   QList -> "'" "(" Expr ")"
 */

ParserError makeError(ParserErrorType Type, int Linum) { return {Type, Linum}; }

ParserError parseSexp(TokenStream *Stream, List **NewSexp) {
  std::list<Object *> Atoms;
  Object *Obj;
  ParserError Err;
  while (hasToken(Stream)) {
    const Token *Token = peekToken(Stream);
    switch (Token->Type) {
    case TOK_OpenParen: {
      takeToken(Stream);
      List *NestedSexp = static_cast<List *>(malloc(sizeof(List *)));
      Err = parseSexp(Stream, &NestedSexp);
      Obj = newSexp(NestedSexp);
      Atoms.push_front(Obj);
    }
    case TOK_CloseParen:
      takeToken(Stream);
      *NewSexp = newList();
      for (auto &Atom : Atoms) {
        listCons(*NewSexp, newCons(Atom));
      }
      return makeError(Parser_Error_No_Error, 0);
    default:
      Err = parseExpr(Stream, &Obj);
      Atoms.push_front(Obj);
    }
  }
}

ParserError parseExpr(TokenStream *Stream, Object **Expr) {
  while (hasToken(Stream)) {
    const Token *Token = takeToken(Stream);
    switch (Token->Type) {
    case TOK_OpenParen: {
      List *Sexp = newList();
      ParserError Err = parseSexp(Stream, &Sexp);
      *Expr = newSexp(Sexp);
      return Err;
    }
    case TOK_Number:
      *Expr = newNum(Token->Val.TokNum);
      return ParserNoError;
    case TOK_String:
      *Expr = newString(Token->Val.TokStr);
      return ParserNoError;
    case TOK_Symbol:
      *Expr = newSymbol(Token->Val.TokStr);
      return ParserNoError;
    }
  }
}

ParserError parseString(char *Input, Object **Prog) {
  std::istringstream Stream(Input);
  TokenStream *TokStream;
  TokStream = lexer(Stream);
  ParserError err = parseExpr(TokStream, Prog);
  return err;
}
