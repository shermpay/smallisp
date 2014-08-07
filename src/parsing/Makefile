# Sherman Pay Jing Hao
# Tuesday, 10. June 2014
# Makefile for lexer/parser and Ast related
# Consider pulling this to overall Makefile
# Flex: smallist.flex
# Bison: grammar.y
# AST: list.c, ast.c
# Execute tests via 'make test_(name)'

CC = clang
CFLAGS = -g -Wall
LIBS = -lfl
TESTDIR = ./tests
EXDIR = ./examples


test_ast: ast_test.c ast.o
	$(CC) $(CFLAGS) $^ -o $(TESTDIR)/$@ && valgrind $(TESTDIR)/$@

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) $< -c

# parser: lex.yy.c grammar.tab.c
# 	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# lexer: lex.yy.c 
# 	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

lex.yy.c: smallisp.flex
	flex $<

grammar.tab.h: grammar.tab.c

grammar.tab.c: grammar.y
	bison -d $<

token.o: token.c token.h
	$(CC) $(CFLAGS) $^ -c

test_token: $(TESTDIR)/test_token.c token.o
	$(CC) $(CFLAGS) -I . $^ -o $(TESTDIR)/$@ && valgrind $(TESTDIR)/$@

lexer: lexer.c token.o
	$(CC) $(CFLAGS) $^ -o $@

test_lexer: lexer
	for file in $(EXDIR)/* ; do \
		echo $$file ; \
		./lexer $$file ; \
		echo "\n" ; \
	done

test_parser: parser
	for file in $(EXDIR)/* ; do \
		echo $$file ; \
		./parser $$file ; \
		echo "\n" ; \
	done

clean:
	rm parser lexer grammar.tab.h grammar.tab.c lex.yy.c
