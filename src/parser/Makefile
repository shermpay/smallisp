# Sherman Pay Jing Hao
# Tuesday, 10. June 2014
# Makefile for lexer/parser and Ast related
# Consider pulling this to overall Makefile
# Flex: smallist.flex
# Bison: grammar.y
# AST: list.c, ast.cc
# Execute tests via 'make test_(name)'

CC=clang
CPP=clang++
CFLAGS=-g -Wall
LIBS=-lfl -ledit -lm
TESTDIR=./tests
EXDIR=./examples

ast.o: ast.cc 
	$(CPP) $(CFLAGS) $< -c

sltypes.o: sltypes.cc 
	$(CPP) $(CFLAGS) $^ -c

list.o: list.cc sltypes.o 
	$(CPP) $(CFLAGS) $^ -c

token.o: token.cc 
	$(CPP) $(CFLAGS) $^ -c

lexer.o: lexer.cc token.o
	$(CPP) $(CFLAGS) $^ -c

lexer: lexer.cc token.o
	$(CPP) $(CFLAGS) $^ -o $@

parser.o: parser.cc lexer.o token.o list.o sltypes.o
	$(CPP) $(CFLAGS) $^ -c

parser: parser.o lexer.o token.o list.o sltypes.o
	$(CPP) $(CFLAGS) $^ -o $@

repl: repl.cc lexer.o token.o
	$(CPP) $(CFLAGS) $^ -ledit -o $@


# ===========
# TESTS
# ===========
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

test_sltypes: $(TESTDIR)/test_sltypes.cc sltypes.o
	$(CPP) $(CFLAGS) $^ -o $(TESTDIR)/bin/$@ && valgrind $(TESTDIR)/bin/$@

test_ast: ast_test.cc ast.o
	$(CPP) $(CFLAGS) $^ -o $(TESTDIR)/bin/$@ && valgrind $(TESTDIR)/bin/$@

test_list: $(TESTDIR)/test_list.cc list.o sltypes.o
	$(CPP) $(CFLAGS) $^ -o $(TESTDIR)/bin/$@ && valgrind $(TESTDIR)/bin/$@

test_token: $(TESTDIR)/test_token.cc token.o
	$(CPP) $(CFLAGS) -I . $^ -o $(TESTDIR)/$@ && valgrind $(TESTDIR)/$@


# Remove test binaries
clean_tests:
	rm $(TESTDIR)/test_list $(TESTDIR)/test_sltypes

clean:
	rm parser lexer *.o
