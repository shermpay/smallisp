# Makefile for lexer/parser
CC = clang
CFLAGS = -g
LIBS = -lfl
TESTDIR = ./tests

slist_test: slist_test.c slist.o
	$(CC) $(CFLAGS) $^ -o $@

slist.o: slist.c slist.h
	$(CC) $(CFLAGS) $< -c

ast_test: ast_test.c ast.o
	$(CC) $(CFLAGS) $^ -o $@

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) $< -c

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
