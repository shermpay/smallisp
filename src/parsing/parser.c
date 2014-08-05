#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum token_type { WHITESPACE,
		  OPEN_PAREN, CLOSE_PAREN,
		  OPEN_BRACK, CLOSE_BRACK,
		  NUMBER, CHAR, SYMBOL,
		  INVALID };

union token_val {
    long tok_num;
    char tok_char;
    char* tok_str;
};
    
typedef struct {
    enum token_type type;
    union token_val val;
} Token;

char *token_tostr(Token *token)
{
    char *str = malloc(30);
    switch (token->type){
	case WHITESPACE:
	    sprintf(str, "(WHITESPACE)");
	    break;
	case OPEN_PAREN:
	    sprintf(str, "(OPEN_PAREN)");
	    break;
	case CLOSE_PAREN:
	    sprintf(str, "(CLOSE_PAREN)");
	    break;
	case OPEN_BRACK:
	    sprintf(str, "(OPEN_BRACK)");
	    break;
	case CLOSE_BRACK:
	    sprintf(str, "(CLOSE_BRACK)");
	    break;
	case SYMBOL:
	    sprintf(str, "(SYMBOL)");
	    /* sprintf(str, "(SYMBOL, %s)", token->val.tok_str); */
	    break;
	case NUMBER:
	    sprintf(str, "(NUMBER, %li)", token->val.tok_num);
	    break;
	case CHAR:
	    sprintf(str, "(CHAR)");
	    /* sprintf(str, "(CHAR, %c)", token->val.tok_char); */
	    break;
	default:
	    sprintf(str,"Unexpected token");
    }
    return str;
}

/* Get value of digit. Returns -1 if not a digit. 
 */
int valof_digit(char n)
{
    if (n < '0' || n > '9') {
	return -1;
    }
    return n - '0';
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
	return 1;
    }
    char *file_name = argv[1];
    FILE *input_file = fopen(file_name, "r");
    char c;
    char* buff = malloc(sizeof(char) * 24);
    while ((c = fgetc(input_file)) != EOF) {
	Token *tok = malloc(sizeof(Token));
	int len = 0;
	printf("%c", c);
	enum token_type tok_type;
	if (isspace(c) || c == ',') {
	    tok_type = WHITESPACE;
	} else if (isalpha(c)) {
	    tok_type = SYMBOL;
	    char nc = fgetc(input_file);
	    while (isalpha(nc)) {
		printf("%c", nc);
		nc = fgetc(input_file);
	    }
	    ungetc(nc, input_file);
	} else {
	    switch (c) {
	    case '(':
		tok_type = OPEN_PAREN;
		break;
	    case ')':
		tok_type = CLOSE_PAREN;
		break;
	    case '[':
		tok_type = OPEN_BRACK;
		break;
	    case ']':
		tok_type = CLOSE_BRACK;
		break;
	    case '0' ... '9':
		tok_type = NUMBER;
		buff[0] = c;
		char nc = fgetc(input_file);
		while (isdigit(nc)) {
		    buff[++len] = nc;
		    printf("%c", nc);
		    nc = fgetc(input_file);
		}
		if (nc != '(' && nc != ')' && !isspace(nc)) {
		    tok_type = INVALID;
		    fprintf(stderr, "Invalid char: %c", nc);
		    return 1;
		}
		long val = 0;
		int mul = 1;
		while (len >= 0) {
		    int v = valof_digit(buff[len--]);
		    val += v * mul;
		    mul *= 10;
		}
		tok->val.tok_num = val;
		ungetc(nc, input_file);
		break;
	    case '\\':
		tok_type = CHAR;
		nc = fgetc(input_file);
		printf("%c", nc);
		break;
	    default:
		tok_type = SYMBOL;
	    }
	}
	tok->type = tok_type;
	char *str = token_tostr(tok);
	printf("\t: %s\n", str);
	free(tok);
	free(str);
    }
    free(buff);
    free(file_name);
    return 0;
}

