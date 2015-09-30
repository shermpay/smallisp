/*
  Author: Sherman Pay
  Version: 0.1; Thursday, 07. August 2014
  Lexer that reads from an input_file, and constructs a TokenStream.
  TODO: Allow arbitary length strings and symbols.
*/
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include "lexer.h"

#define MAX_COLUMN 512		/* Maximum Columns accepted on a line */

static int linum = 1;		/* Keeping track of line number */
static int column = 0;		/* Keeping track of column number */

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
    nc = fgetc(file);
  }
  ungetc(nc, file);
  return valof_numstr(buff, len);
}

/*
  Takes in a file to do lexical analysis on, 
  and produce a token stream via the TokenStream API.
*/
TokenStream* lexer(FILE *input_file)
{
  TokenStream *stream = new_tokenstream();

  char c;
  char nc;

  while ((c = fgetc(input_file)) != EOF) {
    Token *tok = static_cast<Token*>(malloc(sizeof(Token)));
    tok->linum = linum;
    if (c == ';') { // Comment
      tok->type = COMMENT;
      while(c != EOF && c != '\n')
        c = fgetc(input_file);
            
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
      char* buff = static_cast<char*>(malloc(sizeof(char) * 64));
      tok->type = NUMBER;
      buff[0] = c;
      tok->val.tok_num = read_number(input_file, buff);
      free(buff);
    } else if (is_symbolc(c)) { // Symbol
      char* buff = static_cast<char*>(malloc(sizeof(char) * 64));
      if (c == '+' || c == '-') { // Read over one
        ungetc(nc, input_file);
      }
      buff[0] = c;
      tok->type = SYMBOL_TOK;
      read_symbol(input_file, buff); 
      tok->val.tok_str = buff;
    } else if (c == '"') { // String
      char* buff = static_cast<char*>(malloc(sizeof(char) * 64));
      tok->type = STRING;
      read_string(input_file, buff);
      tok->val.tok_str = buff;
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
    push_token(stream, tok);
  }
  fclose(input_file);
  return stream;
}
