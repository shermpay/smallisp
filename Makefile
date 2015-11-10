ARCH := x86
SHELL := /bin/bash
CXX :=clang++
SRC_EXT := cc
HDR_EXT := h

SRC_DIR := ./src
INCL_DIR := ./include
TEST_DIR := ./test
BUILD_DIR := ./build
BIN_DIR := $(BUILD_DIR)/bin

# LLVM_CONFIG ?= llvm-config

# export LIBS := `$(LLVM_CONFIG) --ldflags --libs $(ARCH)`
# LIBS += `$(LLVM_CONFIG) --system-libs`
# ** Remember to include this **
# export LLVM_FLAGS :=`$(LLVM_CONFIG) --cxxflags`

CXX_INCLUDE_FLAGS := -I$(INCL_DIR)
CXX_SAFETY_FLAGS := -Wall
CXX_DEBUG_FLAGS := -g
CXX_INST_FLAGS := --coverage
CXXFLAGS := -std=c++14 $(CXX_INCLUDE_FLAGS) \
	$(CXX_SAFETY_FLAGS) $(CXX_DEBUG_FLAGS) $(CXX_INST_FLAGS)

# Source files
SRCS := $(wildcard  $(SRC_DIR)/*.$(SRC_EXT))

OBJS := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(BUILD_DIR)/%.o, $(SRCS))

# Testing required
TEST_LIBS :=  -lgtest
TEST_BIN_DIR := $(TEST_DIR)/bin

# Code coverage
COV := llvm-cov gcov
COVFLAGS := -f -o $(BUILD_DIR)
PROFDIR := $(TEST_DIR)/profile
COVDIR := $(TEST_DIR)/cov
LCOV := lcov
LCOVFLAGS := -b . -d $(COVDIR) -d $(BUILD_DIR) -c --gcov-tool llvm-gcov --capture
GENHTML := genhtml
GENHTMLFLAGS := --ignore-errors source
HTMLDIR := $(TEST_DIR)/html

# Test files
TEST_SRCS := $(wildcard $(TEST_DIR)/*.$(SRC_EXT))
TEST_BINS := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(TEST_BIN_DIR)/%, $(TEST_SRCS))

main: main.cc $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

allobjs: $(OBJS)

# -----------------------
# Building an object file
# -----------------------
%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $(BUILD_DIR)/$@ -c

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@ -c

# --------------------------
# Building and Running tests
# --------------------------
cov: $(TEST_BINS)
	$(COV) $(COVFLAGS) $(TEST_BIN_DIR)/$*.gcda
	$(LCOV) $(LCOVFLAGS) -o $(COVDIR)/$@
	$(GENHTML) $(GENHTMLFLAGS) $(COVDIR)/$@ -o $(HTMLDIR)/$@

.PHONY: tests
tests: $(TEST_BINS)
	for test in $(TEST_BIN_DIR)/*; do \
		./$$test; \
	done

treewalk_interp_test: $(TEST_DIR)/treewalk_interp_test.cc $(BUILD_DIR)/objects.o $(BUILD_DIR)/list.o $(BUILD_DIR)/reader.o $(BUILD_DIR)/treewalk_interp.o $(BUILD_DIR)/specialforms.o $(BUILD_DIR)/builtins.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(TEST_BIN_DIR)/$@
	@echo "=============== STARTING TESTS ================="
	$(TEST_BIN_DIR)/$@
	mv *.gcda *.gcno $(PROFDIR)

reader_cov: $(TEST_BIN_DIR)/reader_test
	$(COV) $(COVFLAGS) $<.gcda
	$(LCOV) $(LCOVFLAGS) -o $(COVDIR)/$@
	$(GENHTML) $(GENHTMLFLAGS) $(COVDIR)/$@ -o $(HTMLDIR)/$@

reader_test: $(TEST_DIR)/reader_test.cc $(BUILD_DIR)/objects.o $(BUILD_DIR)/list.o  $(BUILD_DIR)/reader.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(TEST_BIN_DIR)/$@
	@echo "=============== STARTING TESTS ================="
	$(TEST_BIN_DIR)/$@
	mv *.gcda *.gcno $(PROFDIR)
list_cov: $(TEST_BIN_DIR)/list_test
	$(COV) $(COVFLAGS) $<.gcda
	$(LCOV) $(LCOVFLAGS) -o $(COVDIR)/$@
	$(GENHTML) $(GENHTMLFLAGS) $(COVDIR)/$@ -o $(HTMLDIR)/$@

list_test: $(TEST_DIR)/list_test.cc $(BUILD_DIR)/list.o $(BUILD_DIR)/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(TEST_BIN_DIR)/$@
	@echo "=============== STARTING TESTS ================="
	$(TEST_BIN_DIR)/$@
	mv *.gcda *.gcno $(PROFDIR)

objects_cov: $(TEST_BIN_DIR)/objects_test
	$(COV) $(COVFLAGS) $<.gcda
	$(LCOV) $(LCOVFLAGS) -o $(COVDIR)/$@
	$(GENHTML) $(GENHTMLFLAGS) $(COVDIR)/$@ -o $(HTMLDIR)/$@

objects_test: $(TEST_DIR)/objects_test.cc $(BUILD_DIR)/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(TEST_BIN_DIR)/$@
	@echo "=============== STARTING TESTS ================="
	$(TEST_BIN_DIR)/$@
	@echo "============ MOVING COVERAGE FILES ============="
	mv *.gcda *.gcno $(PROFDIR)

meta_test: $(TEST_DIR)/meta_test.cc
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $(TEST_BIN_DIR)/$@
	@echo "=============== STARTING TESTS ================="
	$(TEST_BIN_DIR)/$@

.PHONY: clean
clean: 
	rm -r $(BUILD_DIR)/*

.PHONY: clean_cov
clean_cov:
	rm -r $(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.gcno

