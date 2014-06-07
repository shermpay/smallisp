%top{
#include <stdio.h>
}
%x INSTR

CHAR	\\.|\\newline|\\space|\\backspace|\\return|\\formfeed|\\tab
SYMBOL	[[:alpha:]\+\-\*\!\?]+[0-9\-]*|\/
%%
%{
    static int linum = 0;
%}
<INSTR>[.\n]*				ECHO; puts(" -> Matched STRING"); linum++;
<INSTR>"\\\""				ECHO; puts(" -> Matched IN_QUOTE");
<INSTR>"\\\\"				ECHO; puts(" -> Matched BACKSLASH");
<INSTR>"\\n"				ECHO; puts(" -> Matched NEWLINE");
<INSTR>"\\t"				ECHO; puts(" -> Matched TAB");
<INSTR>"\\f"				ECHO; puts(" -> Matched FORM_FEED");
<INSTR>"\\b"				ECHO; puts(" -> Matched BACKSPACE");
<INSTR>"\\r"				ECHO; puts(" -> Matched RETURN");
<INSTR>"\\".				ECHO; puts(" -> INVALID escape character");
<INSTR>\"				{
						ECHO;
						puts(" -> Matched DBL_QUOTE(closing)");
						BEGIN(INITIAL);
					}
[ |\t]*				
\n					linum++;
;.*					ECHO; puts(" -> Matched SEMI"); linum++;
\(					ECHO; puts(" -> Matched LEFT_PAREN");
\)					ECHO; puts(" -> Matched RIGHT_PAREN");
[-+]?[0-9]+				ECHO; puts(" -> Matched NUMBER");
true|false				ECHO; puts(" -> Matched BOOL");
{CHAR}					ECHO; puts(" -> Matched CHAR");
\"					ECHO; puts(" -> Matched DBL_QUOTE"); BEGIN(INSTR);
{SYMBOL}			 	ECHO; puts(" -> Matched SYMBOL");
.					ECHO; puts(" -> Unmatched Delimiter"); /* This is for any unmatched delims*/

%%
int main(int argc, char* argv[])
{
    if (argc < 2) {
	fprintf(stderr, "Need to pass in a smallisp file to analyze\n");
	exit(0);
    }
    FILE *input_file = fopen(argv[1], "r");
    printf("Analyzing file: %s ...", argv[1]);
    yyin = input_file;
    yylex();
}
