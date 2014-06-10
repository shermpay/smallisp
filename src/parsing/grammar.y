%code top {
#include <stdio.h>
#include "sltypes.h"
#include "grammar.tab.h"

    void yyerror(char const *s);
    typedef int myint;
}
%union {
    SlType val;
} 

%define parse.error verbose

%token <val.sl_num> NUMBER
%token <val.sym> SYMBOL
%token SEMI
%token <val.sl_bool> BOOL
%token <val.sl_char> CHAR
%token LPAREN
%token RPAREN
%token DBL_QUOTE
%token ERROR
%token END_OF_FILE 0 "end of file"

%type <val> atom
			
%%
program:
		%empty
	|	sexp
	|	program sexp
		;
sexp:
		atom
	|	LPAREN exp RPAREN { }
		;
		
exp:
		atom {}
	|	atom exp {}
		;
atom:
		NUMBER { $$.sl_num = $1; }
	|	SYMBOL { $$.sym = $1; }
	|	BOOL { $$.sl_bool = $1; }
		;
%%
void yyerror(char const *s)
{
    fprintf(stderr, "Parse Error: %s\n", s);
}
int main(int argc, char* argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Need to pass in a smallisp file to parse\n");
	exit(0);
    }
    FILE *input_file = fopen(argv[1], "r");
    printf("Parsing file: %s ...", argv[1]);
    yyin = input_file;
    yyparse();
    fclose(yyin);
}
