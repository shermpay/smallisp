CC=clang
CFLAGS = -std=c99 -Wall -g
LIBS = -ledit -lm
OBJS = mpc.o lval.o 

repl: repl.c $(OBJS)
	$(CC) $(CFLAGS) repl.c $(OBJS) $(LIBS) -o repl

mpc.o: mpc.c
	$(CC) $(CFLAGS) -c mpc.c

lval.o: lval.c
	$(CC) $(CFLAGS) -c lval.c

clean: *.o repl
	rm *.o
