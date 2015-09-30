/*
  Author: Sherman Pay
  Version: 0.1; Thursday, 07. August 2014
  TokenStreams are implemented as a Doubly Linked List with a small interface.
  Memory usage is not optimized for string outputs.
 */
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *token_tostr(Token *token)
{
    char *str = malloc(64);
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

void flush_stream(TokenStream *stream)
{
    
    StreamNode *old;
    StreamNode *curr = stream->front;
    while(curr != NULL) {
	old = curr;
	curr = curr->next;
	old->next = NULL;
	old->prev = NULL;
	free(old);
    }
    free(curr);
    free(stream);
}

void push_token(TokenStream *stream, Token *token)
{
    StreamNode *node = malloc(sizeof(StreamNode));
    node->token = token;
    node->prev = stream->back;
    node->next = NULL;
    if (stream->back != NULL) {
	stream->back->next = node;
    } else {
	stream->front = node;
    }
    stream->back = node;
}

Token *take_token(TokenStream *stream)
{
    StreamNode *node = stream->front;
    stream->front = stream->front->next;
    node->next = NULL;
    node->prev = NULL;
    Token *result = node->token;
    free(node);
    return result;
}

void ret_token(TokenStream *stream, Token *token)
{
    StreamNode *node = malloc(sizeof(StreamNode));
    node->token = token;
    node->next = stream->front;
    node->prev = NULL;
    stream->front = node;
    if (stream->back == NULL) {
	stream->back = node;
    }
}

bool has_token(TokenStream *stream)
{
    return stream->front != NULL;
}

char* stream_tostr(TokenStream *stream)
{
    int i;
    char *buff = malloc(sizeof(char*) * 256);
    buff[0] = '[';
    buff[1] = '\0';
    for (i = 1; has_token(stream); i++) {
	Token *tok = take_token(stream);
    	char *str = token_tostr(tok);
	strncat(buff, str, 256);
	free(str);
    }
    int len = strlen(buff);
    buff[len - 1] = ']';
    buff[len] = '\0';
    return buff;
}
