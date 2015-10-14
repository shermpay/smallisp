CXX := clang++
CXXFLAGS := -std=c++11 -I../include
TEST_LIBS :=  -lgtest

BIN := ./bin

$(BIN)/ObjectsTest: ObjectsTest.cc ../build/Objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $@
	@echo "=============== STARTING TESTS ================="
	./$@

.PHONY: clean

clean:
	rm ./bin/*
