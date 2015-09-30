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

ParserError makeError(ParserErrorType Type, int Linum) {
  return {Type, Linum};
}

ParserError parseSexp(TokenStream *Stream, List **NewSexp) {
  std::list<Object*> Atoms;
  Object *Obj;
  while (has_token(Stream)) {
    Token *Token = take_token(Stream);
    switch (Token->type) {
      case OPEN_PAREN: {
        List *NestedSexp = static_cast<List*>(malloc(sizeof(List*)));
        parseSexp(Stream, &NestedSexp);
        Obj = newSexp(NestedSexp);
        listCons(*NewSexp, newCons(Obj)); 
      }
      case CLOSE_PAREN:
        *NewSexp = static_cast<List*>(malloc(sizeof(List*)));
        for (auto &Atom : Atoms) {
          listCons(*NewSexp, newCons(Atom));
        }
        return makeError(Parser_Error_No_Error, 0);
      case NUMBER:
        Obj = newNum(Token->val.tok_num);
        Atoms.push_front(Obj);
        break;
      // case STRING:
      //   Obj = newString(Token->val.tok_str);
      //   listCons(CurrSexp, newCons(Obj));
      //   break;
      // case SYMBOL_TOK:
      //   Obj = newSymbol(Token->val.tok_str);
      //   listCons(CurrSexp, newCons(Obj));
      //   break;
      // case INVALID:
      //   break;
    }
  }
}

ParserError parseString(char *Input, List **Prog) {
  std::istringstream Stream(Input);
  TokenStream *TokStream;
  TokStream = lexer(Stream);
  return parseSexp(TokStream, Prog);
}
