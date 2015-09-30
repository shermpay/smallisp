CC=clang
CPP=clang++
CFLAGS=-g -Wall -std=c++11
LIBS=-ledit
TESTDIR=./tests
EXDIR=./examples

PARSER_DIR=parser
PARSER_OBJS=$(PARSER_DIR)/lexer.o $(PARSER_DIR)/parser.o $(PARSER_DIR)/token.o $(PARSER_DIR)/list.o $(PARSER_DIR)/sltypes.o 

repl.o: repl.cc
	$(CPP) $(CFLAGS) $^ $(LIBS) -c

main: main.cc repl.o $(PARSER_OBJS)
	$(CPP) $(CFLAGS) $^ $(LIBS) -o $@
