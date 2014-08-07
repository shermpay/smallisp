#include <stdbool.h>
enum token_type { WHITESPACE,
		  OPEN_PAREN, CLOSE_PAREN,
		  OPEN_BRACK, CLOSE_BRACK,
		  COMMENT, 
		  NUMBER, STRING, SYMBOL,
		  INVALID };

union token_val {
    long tok_num;
    char* tok_str;
};
    
typedef struct {
    int linum;
    enum token_type type;
    union token_val val;
} Token;

char *token_tostr(Token*);

/* Token Stream */
#define STREAM_SIZE (512 * sizeof(Token))
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
Token *next_token(TokenStream*);
void ret_token(TokenStream*, Token*);
bool has_token(TokenStream*); 
char *stream_tostr(TokenStream*);
