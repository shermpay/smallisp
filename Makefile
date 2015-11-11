####################
# Global Variables #
####################
ARCH := x86
SHELL := /bin/bash
CXX :=clang++
SRC_EXT := cc
HDR_EXT := h

###############
# Directories #
###############
SRC_DIR := ./src
INCL_DIR := ./include
TEST_DIR := ./test
BUILD_DIR := ./build
BIN_DIR := ./bin

# LLVM_CONFIG ?= llvm-config

# export LIBS := `$(LLVM_CONFIG) --ldflags --libs $(ARCH)`
# LIBS += `$(LLVM_CONFIG) --system-libs`
# ** Remember to include this **
# export LLVM_FLAGS :=`$(LLVM_CONFIG) --cxxflags`

#############
# CXX Flags #
#############
CXX_INCLUDE_FLAGS := -I$(INCL_DIR)
CXX_SAFETY_FLAGS := -Wall
CXX_DEBUG_FLAGS := -g
CXX_INST_FLAGS := --coverage
CXXFLAGS := -std=c++14 $(CXX_INCLUDE_FLAGS) \
	$(CXX_SAFETY_FLAGS) $(CXX_DEBUG_FLAGS) $(CXX_INST_FLAGS)

################
# Source Files #
################
SRCS := $(wildcard  $(SRC_DIR)/*.$(SRC_EXT))

OBJS := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(BUILD_DIR)/%.o, $(SRCS))

###########
# Testing #
###########
TEST_LIBS :=  -lgtest

#################
# Code coverage #
#################
COV := llvm-cov gcov
COVFLAGS := -f -o $(BUILD_DIR)
PROFDIR := $(TEST_DIR)/profile
COVDIR := $(TEST_DIR)/cov
LCOV := lcov
LCOVFLAGS := -b . -d $(COVDIR) -d $(BUILD_DIR) -c --gcov-tool llvm-gcov --capture
GENHTML := genhtml
GENHTMLFLAGS := --ignore-errors source
HTMLDIR := $(TEST_DIR)/html

##############
# Test files #
##############
TEST_SRCS := $(wildcard $(TEST_DIR)/*.$(SRC_EXT))
TEST_BINS := $(patsubst $(TEST_DIR)/%.$(SRC_EXT), $(BIN_DIR)/%, $(TEST_SRCS))

main: main.cc $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $(BIN_DIR)/$@

allobjs: $(OBJS)

###########################
# Building an object file #
###########################
%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $(BUILD_DIR)/$@ -c

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@ -c

##############################
# Building and Running tests #
##############################
cov: $(TEST_BINS)
	$(COV) $(COVFLAGS) $(BIN_DIR)/$*.gcda
	$(LCOV) $(LCOVFLAGS) -o $(COVDIR)/$@
	$(GENHTML) $(GENHTMLFLAGS) $(COVDIR)/$@ -o $(HTMLDIR)/$@

.PHONY: tests
tests: $(TEST_BINS)
	for test in $(BIN_DIR)/*; do \
		./$$test; \
		mv *.gcda *.gcno $(PROFDIR); \
	done

%.cov: $(BIN_DIR)/%
	$(COV) $(COVFLAGS) $<.gcda
	$(LCOV) $(LCOVFLAGS) -o $(COVDIR)/$@
	$(GENHTML) $(GENHTMLFLAGS) $(COVDIR)/$@ -o $(HTMLDIR)/$@

%: $(BIN_DIR)/%
	$<
	@echo "============ MOVING COVERAGE FILES ============="
	mv *.gcda *.gcno $(PROFDIR)

##################
# Building Tests #
##################
$(BIN_DIR)/treewalk_interp_test: $(TEST_DIR)/treewalk_interp_test.cc $(BUILD_DIR)/objects.o $(BUILD_DIR)/list.o $(BUILD_DIR)/reader.o $(BUILD_DIR)/treewalk_interp.o $(BUILD_DIR)/specialforms.o $(BUILD_DIR)/builtins.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $@

$(BIN_DIR)/reader_test: $(TEST_DIR)/reader_test.cc $(BUILD_DIR)/objects.o $(BUILD_DIR)/list.o  $(BUILD_DIR)/reader.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $@

$(BIN_DIR)/list_test: $(TEST_DIR)/list_test.cc $(BUILD_DIR)/list.o $(BUILD_DIR)/objects.o
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $@

$(BIN_DIR)/objects_test: $(TEST_DIR)/objects_test.cc $(BUILD_DIR)/objects.o
	 $(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $@

$(BIN_DIR)/meta_test: $(TEST_DIR)/meta_test.cc
	$(CXX) $(CXXFLAGS) $(TEST_LIBS) $^ -o $@

.PHONY: clean
clean: 
	rm -r $(BUILD_DIR)/* main

.PHONY: clean_tests
clean_tests:
	rm -r $(BIN_DIR)/*
	rm -r $(COVDIR)/*
	rm -r $(HTMLDIR)/*
	rm -r $(PROFDIR)/*
.PHONY: clean_cov
clean_cov:
	rm -r $(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.gcno

