# Makefile for lexer/parser
CC = clang
CFLAGS = -g
LIBS = -lfl
TESTDIR = ./tests

lexer: lex.yy.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

lex.yy.c: smallisp.flex
	flex $<

tests: lexer
	for file in $(TESTDIR)/* ; do \
		echo $$file ; \
		./lexer $$file ; \
		echo "\n" ; \
	done

clean:	lexer
	rm lexer
