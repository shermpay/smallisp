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
    int linum;
    enum token_type type;
    union token_val val;
} Token;

char *token_tostr(Token *token)
{
    char *str = malloc(30);
    switch (token->type){
	case WHITESPACE:
	    sprintf(str, "(WHITESPACE, ln: %d)", token->linum);
	    break;
	case OPEN_PAREN:
	    sprintf(str, "(OPEN_PAREN, ln: %d)", token->linum);
	    break;
	case CLOSE_PAREN:
	    sprintf(str, "(CLOSE_PAREN, ln: %d)", token->linum);
	    break;
	case OPEN_BRACK:
	    sprintf(str, "(OPEN_BRACK, ln: %d)", token->linum);
	    break;
	case CLOSE_BRACK:
	    sprintf(str, "(CLOSE_BRACK, ln: %d)", token->linum);
	    break;
	case SYMBOL:
	    /* sprintf(str, "(SYMBOL)"); */
	    sprintf(str, "(SYMBOL, %s, ln: %d)", token->val.tok_str, token->linum);
	    break;
	case NUMBER:
	    sprintf(str, "(NUMBER, %li, ln: %d)", token->val.tok_num, token->linum);
	    break;
	case CHAR:
	    sprintf(str, "(CHAR, ln: %d)", token->linum);
	    /* sprintf(str, "(CHAR, %c)", token->val.tok_char); */
	    break;
	default:
	    sprintf(str,"Unexpected token on line: %d", token->linum);
    }
    return str;
}

/* Get value of digit. Returns -1 if not a digit. */
int valof_digit(char n)
{
    if (n < '0' || n > '9') {
	return -1;
    }
    return n - '0';
}

/* Checks if char c is valid symbol char */
int is_symbolc(char c)
{
    return isalpha(c) || c == '*' || c == '+' || c == '-' || c == '/';
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
	return 1;
    }
    char *file_name = argv[1];
    FILE *input_file = fopen(file_name, "r");
    char* buff = malloc(sizeof(char) * 24);
    Token *tok = malloc(sizeof(Token));
    char c;
    int linum = 1;
    while ((c = fgetc(input_file)) != EOF) {
	int len = 0;
	printf("%c", c);
	enum token_type tok_type;
	if (isspace(c) || c == ',') {
	    if (c == '\n')
		linum++;
	    tok_type = WHITESPACE;
	} else if (is_symbolc(c)) {
	    tok_type = SYMBOL;
	    buff[0] = c;
	    char nc = fgetc(input_file);
	    while (is_symbolc(nc)) {
		printf("%c", nc);
		buff[++len] = nc;
		nc = fgetc(input_file);
	    }
	    buff[++len] = '\0';
	    tok->val.tok_str = buff;
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
		tok_type = INVALID;
	    }
	}
	tok->type = tok_type;
	tok->linum = linum;
	char *str = token_tostr(tok);
	printf("\t: %s\n", str);
	free(str);
    }
    free(tok);
    free(buff);
    fclose(input_file);
    return 0;
}

