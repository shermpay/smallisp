CC=clang
CPP=clang++
CFLAGS=-std=c++11 -g -Wall
LIBS=-ledit
TESTDIR=./tests
EXDIR=./examples

STD_DIR=std
STD_OBJS=list.o sltypes.o 

PARSER_DIR=parser
PARSER_OBJS=lexer.o parser.o token.o 

repl.o: repl.cc
	$(CPP) $(CFLAGS) $^ $(LIBS) -c

%.o: %.cc
	$(CPP) $(CFLAGS) $^ $(LIBS) -c

main: main.cc repl.o $(PARSER_OBJS) $(STD_OBJS)
	$(CPP) $(CFLAGS) $^ $(LIBS) -o $@

clean:
	rm main *.o && make -C $(STD_DIR) clean && make -C $(PARSER_DIR) clean
