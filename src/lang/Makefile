CC = clang
CFLAGS = -g -Wall
LIBS = -lfl
TESTDIR = ./tests
EXDIR = ./examples

test_core: $(TESTDIR)/test_core.c core.o
	$(CC) $(CFLAGS) -I . $^ -o $(TESTDIR)/$@ && valgrind $(TESTDIR)/$@

core.o: core.c core.h
	$(CC) $(CFLAGS) $< -c

test_list: $(TESTDIR)/test_list.c list.o
	$(CC) $(CFLAGS) -I . $^ -o $(TESTDIR)/$@ && valgrind $(TESTDIR)/$@

list.o: list.c list.h 
	$(CC) $(CFLAGS) $< -c

test_sltypes: $(TESTDIR)/test_sltypes.c sltypes.o
	$(CC) $(CFLAGS) -I . $^ -o $(TESTDIR)/$@ && valgrind $(TESTDIR)/$@

sltypes.o: sltypes.c
	$(CC) $(CFLAGS) $< -c
