RUN_ARGS := examples/lodPtrTest.mcasm

CXX := g++
CXX_FLAGS := -Wall -lz

TARGET := mcScript
TEST_TARGET := tests
EXE_SUFFIX := .exe

SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := obj

TEST_SRC_DIR := test
TEST_BIN_DIR := test_bin
TEST_OBJ_DIR := test_obj

SRC_FILES = $(shell find src -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

TEST_SRC_FILES = $(shell find test -name '*.cpp')
TEST_OBJ_FILES := $(patsubst $(TEST_SRC_DIR)/%.cpp, $(TEST_OBJ_DIR)/%.o, $(TEST_SRC_FILES))
SRC_OBJ_FILES_WITHOUT_MAIN := $(filter-out $(OBJ_DIR)/$(TARGET).o, $(OBJ_FILES))

all: run

run: build
	$(BIN_DIR)/$(TARGET)$(EXE_SUFFIX) $(RUN_ARGS)

test: build build_test
	$(TEST_BIN_DIR)/$(TEST_TARGET)$(EXE_SUFFIX)

build: $(BIN_DIR) $(OBJ_FILES)
	$(CXX) -o $(BIN_DIR)/$(TARGET)$(EXE_SUFFIX) $(OBJ_FILES) $(CXX_FLAGS)

build_test: $(TEST_BIN_DIR) $(TEST_OBJ_FILES)
	@echo $(SRC_OBJ_FILES_WITHOUT_MAIN)
	$(CXX) -o $(TEST_BIN_DIR)/$(TEST_TARGET)$(EXE_SUFFIX) $(TEST_OBJ_FILES) $(SRC_OBJ_FILES_WITHOUT_MAIN)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< -o $@ $(CXX_FLAGS)

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -Iinclude -Itest -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TEST_BIN_DIR):
	mkdir -p $(TEST_BIN_DIR)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(TEST_BIN_DIR)
	rm -rf $(TEST_OBJ_DIR)