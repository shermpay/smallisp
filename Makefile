CC=clang++
CFLAGS=-std=c++11 -Wall -g
LIBS=-ledit -lm
SRC_DIR=./src
BIN_DIR=./bin
OBJ_DIR=./obj

STD_OBJS=$(OBJ_DIR)/list.o $(OBJ_DIR)/sltypes.o 
PARSER_OBJS=$(OBJ_DIR)/lexer.o $(OBJ_DIR)/parser.o $(OBJ_DIR)/token.o 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) $(CFLAGS) $^ -c -o $@

main: $(SRC_DIR)/main.cc $(STD_OBJS) $(PARSER_OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $(BIN_DIR)/$ 

clean: 
	rm -r $(OBJ_DIR)/*.o
