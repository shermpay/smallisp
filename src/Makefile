ARCH=X86
CC=clang
CXX=clang++
LLVM_CONFIG=llvm-config
CXXFLAGS=-std=c++11 `$(LLVM_CONFIG) --cxxflags` -Wall -g 
LIBS=`$(LLVM_CONFIG) --libs core` `$(LLVM_CONFIG) --ldflags` -ldl -lcurses -ledit -lm -pthread
TESTDIR=./tests
EXDIR=./examples

STD_DIR=std
STD_OBJS=list.o sltypes.o 

PARSER_DIR=parser
PARSER_OBJS=lexer.o parser.o token.o 

CODEGEN_OBJS=llvm_code_gen.o

repl.o: repl.cc
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -c

%.o: %.cc
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -c

main: main.cc repl.o $(PARSER_OBJS) $(STD_OBJS) $(CODEGEN_OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

clean:
	rm main *.o && make -C $(STD_DIR) clean && make -C $(PARSER_DIR) clean
