ARCH=x86
CC=clang++
LLVM_CONFIG=llvm-config
CXXFLAGS=-std=c++11 `$(LLVM_CONFIG) --cxxflags` -Wall -g 
LIBS=`$(LLVM_CONFIG) --ldflags --libs $(ARCH) support` -ledit -lm
SRC_DIR=./src
BIN_DIR=./bin
OBJ_DIR=./obj

STD_OBJS=$(OBJ_DIR)/list.o $(OBJ_DIR)/sltypes.o 
PARSER_OBJS=$(OBJ_DIR)/lexer.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/token.o 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $^ -c -o $@

main: $(SRC_DIR)/main.cc $(STD_OBJS) $(PARSER_OBJS)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $(BIN_DIR)/$ 

clean: 
	rm -r $(OBJ_DIR)/*.o
