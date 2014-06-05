#include <stdio.h>
#include <stdlib.h>

typedef enum { OPEN_PAREN, CLOSE_PAREN, NUMBER, SYMBOL } token_type;

char *token_name(token_type token)
{
    switch (token){
	case OPEN_PAREN:
	    return "OPEN_PAREN";
	case CLOSE_PAREN:
	    return "CLOSE_PAREN";
	case SYMBOL:
	    return "SYMBOL";
	default:
	    return "Unexpected token";
    }
}


char *next_token(char *buffer)
{
    return "";
}

int main(int argc, char *argv[])
{
    char *file_name = argv[1];
    FILE *input_file = fopen(file_name, "r");
    char c;

    while ((c = fgetc(input_file)) != EOF) {
	printf("%c: ", c);
	token_type token;
	switch (c) {
	    case '(':
		token = OPEN_PAREN;
		break;
	    case ')':
		token = CLOSE_PAREN;
		break;
	    
	    default:
		token = SYMBOL;
	}
	printf("%s\n", token_name(token));
    }
    return 0;
}

