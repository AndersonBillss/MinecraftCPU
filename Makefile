CXX := g++
CXX_FLAGS := -Wall

SRC_DIR := src
TARGET := mcScript

OBJ_DIR := obj
BIN_DIR := bin
EXE_SUFFIX := .exe

all: run

run: build
	$(BIN_DIR)/$(TARGET)

build: $(BIN_DIR)
	$(CXX) $(SRC_DIR)/$(TARGET).cpp -o $(BIN_DIR)/$(TARGET)$(EXE_SUFFIX)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)