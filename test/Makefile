CXX := clang++
CXXFLAGS := -std=c++11 -I../include
TEST_LIBS :=  -lgtest

BIN := ./bin

$(BIN)/objects_test: objects_test.cc ../build/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $@
	@echo "=============== STARTING TESTS ================="
	./$@

$(BIN)/list_test: list_test.cc ../build/list.o ../build/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $@
	@echo "=============== STARTING TESTS ================="
	./$@

.PHONY: clean

clean:
	rm ./bin/*
