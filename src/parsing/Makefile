# Sherman Pay Jing Hao
# Tuesday, 10. June 2014
# Makefile for lexer/parser and Ast related
# Flex: smallist.flex
# Bison: grammar.y
# AST: list.c, ast.c
# Execute tests via 'make test_(name)'

CC = clang
CFLAGS = -g -Wall
LIBS = -lfl
TESTDIR = ./tests
EXDIR = ./examples

test_list: $(TESTDIR)/test_list.c list.o
	$(CC) $(CFLAGS) -I . $^ -o $(TESTDIR)/$@ && valgrind $(TESTDIR)/$@

list.o: list.c list.h 
	$(CC) $(CFLAGS) $< -c

test_sltypes: $(TESTDIR)/test_sltypes.c sltypes.o
	$(CC) $(CFLAGS) -I . $^ -o $(TESTDIR)/$@ && valgrind $(TESTDIR)/$@

sltypes.o: sltypes.c
	$(CC) $(CFLAGS) $< -c

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

lexer: lexer.c
	$(CC) $(CFLAGS) $< -o $@

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
