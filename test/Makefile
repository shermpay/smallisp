CXX := clang++
CXXFLAGS := -std=c++11 -I../include
TEST_LIBS :=  -lgtest

BIN := ./bin
BUILD := ../build

all: objects_test list_test reader_test

reader_test: reader_test.cc $(BUILD)/objects.o $(BUILD)/list.o  $(BUILD)/reader.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(BIN)/$@
	@echo "=============== STARTING TESTS ================="
	$(BIN)/$@

list_test: list_test.cc $(BUILD)/list.o $(BUILD)/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(BIN)/$@
	@echo "=============== STARTING TESTS ================="
	$(BIN)/$@

objects_test: objects_test.cc $(BUILD)/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(BIN)/$@
	@echo "=============== STARTING TESTS ================="
	$(BIN)/$@

meta_test: meta_test.cc
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(BIN)/$@
	@echo "=============== STARTING TESTS ================="
	$(BIN)/$@

.PHONY: clean

clean:
	rm ./bin/*
