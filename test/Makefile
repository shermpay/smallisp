# C++ compiler and flags
CXX := clang++
CXXFLAGS := -std=c++14 -I../include --coverage
TEST_LIBS :=  -lgtest

# Directories
BIN := ./bin
BUILD := ../build

# Code coverage
COV := llvm-cov gcov
COVFLAGS := -f -o $(BUILD)
PROFDIR := ./profile
COVDIR := ./cov
LCOV := lcov
LCOVFLAGS := -b ../ -d $(COVDIR) -d $(BUILD) -c --gcov-tool llvm-gcov --capture
GENHTML := genhtml
GENHTMLFLAGS := --ignore-errors source
HTMLDIR := ./html

all: objects_test list_test reader_test

reader_test: reader_test.cc $(BUILD)/objects.o $(BUILD)/list.o  $(BUILD)/reader.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(BIN)/$@
	@echo "=============== STARTING TESTS ================="
	$(BIN)/$@

list_test: list_test.cc $(BUILD)/list.o $(BUILD)/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(BIN)/$@
	@echo "=============== STARTING TESTS ================="
	$(BIN)/$@

objects_cov: $(BIN)/objects_test
	$(COV) $(COVFLAGS) $<.gcda
	$(LCOV) $(LCOVFLAGS) -o $(COVDIR)/$@
	$(GENHTML) $(GENHTMLFLAGS) $(COVDIR)/$@ -o $(HTMLDIR)/$@

$(BIN)/objects_test: objects_test

objects_test: objects_test.cc $(BUILD)/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(BIN)/$@
	@echo "=============== STARTING TESTS ================="
	$(BIN)/$@
	@echo "============ MOVING COVERAGE FILES ============="
	mv *.gcda *.gcno $(PROFDIR)

meta_test: meta_test.cc
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(BIN)/$@
	@echo "=============== STARTING TESTS ================="
	$(BIN)/$@

.PHONY: clean

clean:
	rm ./bin/* $(COVDIR)/* $(PROFDIR)/*
