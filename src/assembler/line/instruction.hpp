#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "../operand/IOperand.hpp"
#include "../operand/Int8Operand.hpp"
#include "../operand/RegOperand.hpp"
#include "../operand/MemOperand.hpp"

struct instruction
{
    std::string operatorBin;
    std::vector<IOperand *> operands;
};

RegOperand *REG = new RegOperand();
Int8Operand *INT = new Int8Operand();
MemOperand *INT = new MemOperand();

std::unordered_map<std::string, instruction> instructionTable = {
    // 0 - 15: Register/Program Control
    // 00000000 - 00001111
    // ------------------------------------
    {"NOP", {operatorBin : "00000000", operands : {}}},
    {"LDI", {operatorBin : "00000001", operands : {REG, INT}}},
    {"ADD_IMM", {operatorBin : "00000010", operands : {REG, INT}}},
    {"SUB_IMM", {operatorBin : "00000011", operands : {REG, INT}}},
};