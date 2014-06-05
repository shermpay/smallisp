CC=clang
CFLAGS = -std=c99 -Wall -g
LIBS = -ledit -lm
OBJS = mpc.o lval.o lenv.o lbuiltin.o
SRC_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = ./obj

all: repl.c $(OBJS)
	$(CC) $(CFLAGS) $< $(OBJS) $(LIBS) -o repl

mpc.o: mpc.c
	$(CC) $(CFLAGS) -c $<

lval.o: lval.c ltypes.h
	$(CC) $(CFLAGS) -c $<

lenv.o: lenv.c ltypes.h
	$(CC) $(CFLAGS) -c $<

lbuiltin.o: lbuiltin.c ltypes.h
	$(CC) $(CFLAGS) -c $<

clean: 
	rm -r $(OBJ_DIR)/*.o
