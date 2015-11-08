export ARCH := x86
export CXX :=clang++
export SRC_EXT := cc
export HDR_EXT := h

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

export CXX_INCLUDE_FLAGS := -I$(INCL_DIR)
export CXX_SAFETY_FLAGS := -Wall
export CXX_DEBUG_FLAGS := -g
export CXX_INST_FLAGS := --coverage
export CXXFLAGS := -std=c++14 $(CXX_INCLUDE_FLAGS) \
	$(CXX_SAFETY_FLAGS) $(CXX_DEBUG_FLAGS)

export SRCS := $(wildcard  $(SRC_DIR)/*.$(SRC_EXT))

export OBJS := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(BUILD_DIR)/%.o, $(SRCS))

main: main.cc $(OBJS)
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@

allobjs: $(OBJS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $@ -c

%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) $(LIBS) $^ -o $(BUILD_DIR)/$@ -c

.PHONY: clean
clean: 
	rm -r $(BUILD_DIR)/*

.PHONY: clean_cov
clean_cov:
	rm -r $(BUILD_DIR)/*.gcda $(BUILD_DIR)/*.gcno

