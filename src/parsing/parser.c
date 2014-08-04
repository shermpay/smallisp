#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum token_type { WHITESPACE,
		  OPEN_PAREN, CLOSE_PAREN,
		  OPEN_BRACK, CLOSE_BRACK,
		  NUMBER, CHAR, SYMBOL,
		  INVALID };

union token_val {
    long sl_num;
    char sl_char;
    char* sl_string;
};
    
struct Token {
    enum token_type type;
    union token_val val;
};

char *token_tostr(struct Token token)
{
    switch (token.type){
	case WHITESPACE:
	    return "(WHITESPACE)";
	case OPEN_PAREN:
	    return "(OPEN_PAREN)";
	case CLOSE_PAREN:
	    return "(CLOSE_PAREN)";
	case OPEN_BRACK:
	    return "(OPEN_BRACK)";
	case CLOSE_BRACK:
	    return "(CLOSE_BRACK)";
	case SYMBOL:
	    return "(SYMBOL, )";
	case NUMBER:
	    return "(NUMBER, )";
	case CHAR:
	    return "(CHAR, )";
	default:
	    return "Unexpected token";
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
	return 1;
    }
    char *file_name = argv[1];
    FILE *input_file = fopen(file_name, "r");
    char c;
while ((c = fgetc(input_file)) != EOF) {
	printf("%c", c);
	enum token_type token;
	if (isspace(c) || c == ',') {
	    token = WHITESPACE;
	} else if (isalpha(c)) {
	    token = SYMBOL;
	    char nc = fgetc(input_file);
	    while (isalpha(nc)) {
		printf("%c", nc);
		nc = fgetc(input_file);
	    }
	    ungetc(nc, input_file);
	} else {
	    switch (c) {
	    case '(':
		token = OPEN_PAREN;
		break;
	    case ')':
		token = CLOSE_PAREN;
		break;
	    case '[':
		token = OPEN_BRACK;
		break;
	    case ']':
		token = CLOSE_BRACK;
		break;
	    case '0' ... '9':
		token = NUMBER;
		char nc = fgetc(input_file);
		while (isdigit(nc)) {
		    printf("%c", nc);
		    nc = fgetc(input_file);
		}
		if (nc != '(' && nc != ')' && !isspace(nc)) {
		    token = INVALID;
		    fprintf(stderr, "Invalid char: %c", nc);
		    return 1;
		}
		ungetc(nc, input_file);
		break;
	    case '\\':
		token = CHAR;
		nc = fgetc(input_file);
		printf("%c", nc);
		break;
	    default:
		token = SYMBOL;
	    }
	}
	printf("\t: %s\n", token_tostr(token));
    }
    return 0;
}

