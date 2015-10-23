CXX := clang++
CXXFLAGS := -std=c++11 -I../include
TEST_LIBS :=  -lgtest

BIN := ./bin

list_test: list_test.cc ../build/list.o ../build/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(BIN)/$@
	@echo "=============== STARTING TESTS ================="
	$(BIN)/$@

objects_test: objects_test.cc ../build/objects.o
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
