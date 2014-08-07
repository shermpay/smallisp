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
    stream_tostr(stream);
    for (i = 0; i < len; i++) {
	free(arr[i]);
    }
}
