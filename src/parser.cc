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
  while (has_token(Stream)) {
    Token *Token = peek_token(Stream);
    switch (Token->type) {
    case OPEN_PAREN: {
      take_token(Stream);
      List *NestedSexp = static_cast<List *>(malloc(sizeof(List *)));
      Err = parseSexp(Stream, &NestedSexp);
      Obj = newSexp(NestedSexp);
      Atoms.push_front(Obj);
    }
    case CLOSE_PAREN:
      take_token(Stream);
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
  while (has_token(Stream)) {
    Token *Token = take_token(Stream);
    switch (Token->type) {
    case OPEN_PAREN: {
      List *Sexp = newList();
      ParserError Err = parseSexp(Stream, &Sexp);
      *Expr = newSexp(Sexp);
      return Err;
    }
    case NUMBER:
      *Expr = newNum(Token->val.tok_num);
      return ParserNoError;
    case STRING:
      *Expr = newString(Token->val.tok_str);
      return ParserNoError;
    case SYMBOL_TOK:
      *Expr = newSymbol(Token->val.tok_str);
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
