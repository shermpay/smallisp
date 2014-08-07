#include <token.h>
#include <stdio.h>
char *token_tostr(Token *token)
{
    char *str = malloc(128);
    switch (token->type){
	case COMMENT:
	    sprintf(str, "(COMMENT, ln: %d)", token->linum);
	    break;
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
	    sprintf(str, "(SYMBOL, %s, ln: %d)", token->val.tok_str, token->linum);
	    break;
	case NUMBER:
	    sprintf(str, "(NUMBER, %li, ln: %d)", token->val.tok_num, token->linum);
	    break;
	case STRING:
	    sprintf(str, "(STRING, %s, ln: %d)", token->val.tok_str, token->linum);
	    break;
	default:
	    sprintf(str,"Unexpected token on line: %d", token->linum);
    }
    return str;
}

TokenStream* new_tokenstream(void)
{
    TokenStream *stream = malloc(sizeof(TokenStream)); 
    stream->front = NULL;
    stream->back = NULL;
    return stream;
}

TokenNode* new_tokennode(TokenNode* next)
{
    TokenNode *node = malloc(sizeof(TokenNode));
    stream->next = next;
    stream->prev = NULL;
    return node;
}

void push_token(TokenStream *stream, Token *token)
{
}
