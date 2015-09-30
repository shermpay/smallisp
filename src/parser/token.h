/*
  Author: Sherman Pay
  Version: 0.1; Thursday, 07. August 2014
  Token and TokenStream definitions here.
 */
#include <stdbool.h>
enum token_type { WHITESPACE,
		  OPEN_PAREN, CLOSE_PAREN,
		  OPEN_BRACK, CLOSE_BRACK,
		  COMMENT, 
		  NUMBER, STRING, SYMBOL_TOK,
		  INVALID };

union token_val {
    long tok_num;
    char* tok_str;
};
    
typedef struct {
    int linum;			/* Aid error reporting */
    enum token_type type;
    union token_val val;
} Token;

char *token_tostr(Token*);

/* 
   Token Stream
   ------------
   Provides a simple API to interact with tokens via a FIFO Stream.
   You build a stream by pushing tokens onto the stream.

   And you can manipulate the tokens via the following functions:
   - take_token(TokenStream*) -> Token* : Take the next token from the stream
   - has_token(TokenStream*) -> bool : Check if the stream has any tokens
   - ret_token(TokenStream*) -> void : Return the last token you took from the stream
   ret_token technically allows you to insert anything to the front of the token stream.
   
   Finally you construct a stream with new_tokenstream(void), and flush/close the stream,
   with flush_stream(TokenStream*).
*/
typedef struct stream_node StreamNode;
struct stream_node {
    Token *token;
    StreamNode *next;
    StreamNode *prev;
};

typedef struct {
    StreamNode *front;
    StreamNode *back;
} TokenStream;

char *token_tostr(Token*);
TokenStream* new_tokenstream(void);
void flush_stream(TokenStream*);
void push_token(TokenStream*, Token*);
Token *take_token(TokenStream*);
void ret_token(TokenStream*, Token*);
bool has_token(TokenStream*); 
char *stream_tostr(TokenStream*);
