#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "operand/iOperand.hpp"
#include "operand/int8Operand.hpp"
#include "operand/regOperand.hpp"
#include "operand/memOperand.hpp"

struct Instruction
{
    std::string operatorBin;
    std::vector<IOperand *> operands;
};

RegOperand *REG = new RegOperand();
MemOperand *MEM = new MemOperand();
Int8Operand *INT = new Int8Operand();

std::unordered_map<std::string, Instruction> instructionTable = {
    // 0 - 15: Register/Program Control
    // 00000000 - 00001111
    // ------------------------------------
    {"NOP", {operatorBin : "00000000", operands : {}}},
    {"LDI", {operatorBin : "00000001", operands : {REG, INT}}},
    {"ADD_IMM", {operatorBin : "00000010", operands : {REG, INT}}},
    {"SUB_IMM", {operatorBin : "00000011", operands : {REG, INT}}},
    {"JMP_IMM", {operatorBin : "00000100", operands : {INT}}},
    {"BGT_IMM", {operatorBin : "00000101", operands : {INT}}},
    {"BLT_IMM", {operatorBin : "00000110", operands : {INT}}},
    {"BEQ_IMM", {operatorBin : "00000111", operands : {INT}}},
    {"JMP_REG", {operatorBin : "00001000", operands : {REG}}},
    {"CALL", {operatorBin : "00001001", operands : {INT}}},
    {"RET", {operatorBin : "00001010", operands : {}}},

    // 16 - 31: ALU Operations
    // 00010000 - 00011111
    // ------------------------------------
    {"COPY", {operatorBin : "00010000", operands : {REG, REG}}},
    {"ADD", {operatorBin : "00010001", operands : {REG, REG}}},
    {"SUB", {operatorBin : "00010010", operands : {REG, REG}}},
    {"OR", {operatorBin : "00010011", operands : {REG, REG}}},
    {"AND", {operatorBin : "00010100", operands : {REG, REG}}},
    {"XOR", {operatorBin : "00010101", operands : {REG, REG}}},
    {"NOR", {operatorBin : "00010110", operands : {REG, REG}}},
    {"NAND", {operatorBin : "00010111", operands : {REG, REG}}},
    {"XNOR", {operatorBin : "00011000", operands : {REG, REG}}},
    {"NOT", {operatorBin : "00011001", operands : {REG, REG}}},
    {"RSHFT", {operatorBin : "00011010", operands : {REG}}},
    {"LSHFT", {operatorBin : "00011011", operands : {REG}}},
    {"CMP", {operatorBin : "00011100", operands : {REG, REG}}},

    // 32 - 47: Advanced Math Functions
    // 00100000 - 00101111
    // ------------------------------------
    {"MUL", {operatorBin : "00100000", operands : {REG, REG}}},
    {"DIV", {operatorBin : "00100001", operands : {REG, REG}}},
    {"SIN", {operatorBin : "00100010", operands : {REG}}},
    {"COS", {operatorBin : "00100011", operands : {REG}}},

    // 48 - 63: RAM
    // 00110000 - 00111111
    // ------------------------------------
    {"STR", {operatorBin : "00110000", operands : {MEM, REG}}},
    {"LOD", {operatorBin : "00110001", operands : {REG, MEM}}},
    {"STR_PTR", {operatorBin : "00110010", operands : {REG, REG}}},
    {"LOD_PTR", {operatorBin : "00110011", operands : {REG, REG}}},
    
    // 64 - 127: Graphics/System Calls
    // 01000000 - 01111111
    // ------------------------------------
    {"SHOW", {operatorBin : "01000000", operands : {REG}}},
};