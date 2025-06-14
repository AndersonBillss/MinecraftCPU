CXX := g++
CXX_FLAGS := -Wall

SRC_DIR := src
TARGET := mcScript

OBJ_DIR := obj
BIN_DIR := bin
EXE_SUFFIX := .exe

SRC_FILES = $(shell find src -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: run

run: build
	$(BIN_DIR)/$(TARGET)$(EXE_SUFFIX)

build: $(BIN_DIR) $(OBJ_FILES)
	$(CXX) -o $(BIN_DIR)/$(TARGET)$(EXE_SUFFIX) $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)