%top{
#include <stdio.h>
#include "grammar.tab.h"
}
%x INSTR

CHAR	\\.|\\newline|\\space|\\backspace|\\return|\\formfeed|\\tab
SYMBOL	[[:alpha:]\+\-\*\!\?]+[0-9\-]*|\/
%%
<INSTR>[.\n]*				 
<INSTR>"\\\""				 
<INSTR>"\\\\"				 
<INSTR>"\\n"				 
<INSTR>"\\t"				 
<INSTR>"\\f"				 
<INSTR>"\\b"				 
<INSTR>"\\r"				 
<INSTR>"\\".				 
<INSTR>\"				{
    
    BEGIN(INITIAL);
 }
[ |\t]*				
\n					
;.*					 
\(					  return LPAREN;
\)					  return RPAREN;
[-+]?[0-9]+				{
    fprintf(stderr, "%s\n", yytext);
    yylval.num = atoi(yytext);
    return NUMBER;
    
} 
true|false				  return BOOL;
{CHAR}					  return CHAR;
\"					  BEGIN(INSTR);
{SYMBOL}			 	{
    fprintf(stderr, "%s\n", yytext);
    yylval.sym = strdup(yytext); return SYMBOL;
    
}
.					puts(" -> Unmatched Delimiter"); 
<<EOF>>					puts("END OF FILE"); return END_OF_FILE;
%%
/* int main(int argc, char* argv[]) */
/* { */
/*     if (argc < 2) { */
/* 	fprintf(stderr, "Need to pass in a smallisp file to analyze\n"); */
/* 	exit(0); */
/*     } */
/*     FILE *input_file = fopen(argv[1], "r"); */
/*     printf("Analyzing file: %s ...", argv[1]); */
/*     yyin = input_file; */
/*     yylex(); */
/*     fclose(yyin); */
/* } */
