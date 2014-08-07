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
struct token_node;
typedef struct {
    struct token_node *next;
    struct token_node *prev;
} TokenNode;

typedef struct {
    TokenNode *front;
    TokenNode *back;
} TokenStream;

TokenStream* new_tokenstream(void);
void push_token(TokenStream*, Token*);
Token *next_token(TokenSream*);
void unget_token(TokenStream*, Token*);
bool has_token(TokenStream*); 

