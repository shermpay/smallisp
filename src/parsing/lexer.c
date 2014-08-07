#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <token.h>

#define MAX_COLUMN 512

static int linum = 1;
static int column = 0;

/* Get value of digit. Returns -1 if not a digit. */
int valof_digit(char n)
{
    if (n < '0' || n > '9') {
	return -1;
    }
    return n - '0';
}

/* Gets the value of a number string */
long valof_numstr(char *numstr, int len)
{
    long val = 0;
    int mul = 1;
    while (len > 0) {
	int v = valof_digit(numstr[len--]);
	val += v * mul;
	mul *= 10;
    }
    if (isdigit(numstr[0])) {
	val += mul * valof_digit(numstr[0]);
	return val;
    } else {
	if (numstr[0] == '-') {
	    val *= -1;
	}
	return val;
    }
}

/* Given a character returns the escape sequence of it */
char escape_char(char c) {
    switch (c) {
	case 'n':
	    return '\n';
	case 't':
	    return '\t';
	case 'r':
	    return '\r';
	case 'b':
	    return '\b';
	case 'v':
	    return '\v';
	default:
	    return c;
    }
}

/* 
 * Checks if char c is valid symbol char 
 */
int is_symbolc(char c)
{
    return isalpha(c) || c == '*' || c == '+' || c == '-' || c == '/';
}

/* 
 *  Reads in an entire symbol from file and stores it into buff. 
 *  Returns an int for the length of the symbol.
 *  Symbols are determined via the is_symbolc function.
 */
int read_symbol(FILE *file, char *buff)
{
    char nc = fgetc(file);
    int len = 0;
    while (is_symbolc(nc)) {
	printf("%c", nc);
	buff[++len] = nc;
	nc = fgetc(file);
    }
    buff[++len] = '\0';
    ungetc(nc, file);
    return len--;
}

/* 
 *  Reads in an entire string from file and stores it into buff. 
 *  Length of string is stored in the front of buff.
 *  Returns an int for the length of the string.
 */
int read_string(FILE *file, char *buff){
    char nc = fgetc(file);
    int len = 0;
    while (nc != '"') {
	if (nc == '\n') {
	    linum++;
	    column = 0;
	}
	if (nc == '\\') {
	    nc = fgetc(file);
	    nc = escape_char(nc);
	}
	printf("%c", nc);
	buff[++len] = nc;
	nc = fgetc(file);
    }
    buff[++len] = '\0';	// Terminate string 
    buff[0] = len--;	// Store length at front of string
    return len;
}

/*
 * Reads in a number from the file and stores it into buff.
 * Returns a long representing the number.
 */
long read_number(FILE *file, char *buff)
{
    char nc = fgetc(file);
    int len = 0;
    while (isdigit(nc)) {
	buff[++len] = nc;
	printf("%c", nc);
	nc = fgetc(file);
    }
    ungetc(nc, file);
    return valof_numstr(buff, len);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
	return 1;
    }
    char *file_name = argv[1];
    FILE *input_file = fopen(file_name, "r");
    char* buff = malloc(sizeof(char) * 128);
    Token *tok = malloc(sizeof(Token));
    char c;
    char nc;
    while ((c = fgetc(input_file)) != EOF) {
	printf("%c", c);
	tok->linum = linum;
	if (c == ';') { // Comment
	    tok->type = COMMENT;
	    size_t line_len;
	    fgetln(input_file, &line_len);
	    linum++;
	    column = 0;
	} else if (isspace(c) || c == ',') { // Whitespace
	    if (c == '\n') {
		linum++;
		column = 0;
	    }
	    tok->type = WHITESPACE;
	} else if (isdigit(c) || 
		   ((c == '+' || c == '-') && isdigit(nc = fgetc(input_file)))) {
	    if (c == '+' || c == '-') { // Read over one
		ungetc(nc, input_file);
	    }
	    tok->type = NUMBER;
	    buff[0] = c;
	    tok->val.tok_num = read_number(input_file, buff);
	} else if (is_symbolc(c)) { // Symbol
	    if (c == '+' || c == '-') { // Read over one
		ungetc(nc, input_file);
	    }
	    buff[0] = c;
	    tok->type = SYMBOL;
	    read_symbol(input_file, buff); 
	    tok->val.tok_str = buff;
	} else if (c == '"') { // String
	    tok->type = STRING;
	    read_string(input_file, buff);
	    tok->val.tok_str = buff;
	    printf("\"");
	} else {
	    switch (c) { // Delimiters
		case '(':
		    tok->type = OPEN_PAREN;
		    break;
		case ')':
		    tok->type = CLOSE_PAREN;
		    break;
		case '[':
		    tok->type = OPEN_BRACK;
		    break;
		case ']':
		    tok->type = CLOSE_BRACK;
		    break;
		default:
		    tok->type = INVALID;
	    }
	}
	column++;
	char *str = token_tostr(tok);
	printf("\t: %s\n", str);
	free(str);
    }
    free(tok);
    free(buff);
    fclose(input_file);
    return 0;
}

