#include <stdlib.h>
#include <stdio.h>
#include "token.h"

int main(int argc, char *argv[])
{
    int len = 10;
    Token *arr[len];
    int i;
    for (i = 0; i < len; i++) {
    	arr[i] = malloc(sizeof(Token));
    	arr[i]->type = COMMENT;
    	arr[i]->linum = i;
    }

    for (i = 0; i < len; i++) {
    	char *str = token_tostr(arr[i]);
    	printf("%s, ", str);
    	free(str);
    }
    
    TokenStream *stream = new_tokenstream();
    for (i = 0; i < len; i++) {
    	push_token(stream, arr[i]);
    }
    char *str = stream_tostr(stream);
    printf("%s\n", str);
    free(str);
    for (i = 0; i < len; i++) {
    	free(arr[i]);
    }
    free(arr);

    flush_stream(stream);
    Token *t1 = malloc(sizeof(Token));
    t1->type = SYMBOL;
    t1->linum = 0;
    t1->val.tok_str = "defn";
    Token *t2 = malloc(sizeof(Token));
    t2->type = SYMBOL;
    t2->linum = 1;
    t2->val.tok_str = "map";
    Token *t3 = malloc(sizeof(Token));
    t3->type = SYMBOL;
    t3->linum = 2;
    t3->val.tok_str = "reduce";
    printf("1) %s", token_tostr(t1));
    printf("2) %s", token_tostr(t2));
    printf("3) %s", token_tostr(t3));

    TokenStream *s2 = new_tokenstream();
    push_token(s2, t1);
    push_token(s2, t2);
    push_token(s2, t3);
    str = stream_tostr(s2);
    printf("%s\n", str);
    free(str);
    flush_stream(s2);
    free(t1);
    free(t2);
    free(t3);
}
