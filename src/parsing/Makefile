# Makefile for lexer/parser
CC = clang
CFLAGS = -g
LIBS = -lfl
TESTDIR = ./tests

parser: lex.yy.c grammar.tab.c
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

lexer: lex.yy.c 
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

lex.yy.c: smallisp.flex
	flex $<

grammar.tab.h: grammar.tab.c

grammar.tab.c: grammar.y
	bison -d $<

tests: lexer
	for file in $(TESTDIR)/* ; do \
		echo $$file ; \
		./lexer $$file ; \
		echo "\n" ; \
	done

clean:
	rm parser grammar.tab.h grammar.tab.c lex.yy.c
