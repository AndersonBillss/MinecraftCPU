#include "assembler.hpp"
#include "tokenizer/tokenizer.hpp"
#include "instruction.hpp"
#include "../utils/syntaxError.hpp"

#include <iostream>

std::string Assembler::compile(const std::string &sourceCode)
{
    std::tuple<Tokenizer::SymbolMap, Tokenizer::InstructionList> tokenized = Tokenizer::tokenize(sourceCode);
    Tokenizer::SymbolMap symbols = std::get<0>(tokenized);
    Tokenizer::InstructionList instructions = std::get<1>(tokenized);

    std::string compiled = "";

    for (std::vector<std::string> instruction : instructions)
    {
        std::string opCode = instruction[0];

        auto it = instructionTable.find(opCode);
        if (it == instructionTable.end())
        {
            throw SyntaxError("Opcode: " + opCode + " does not exist.");
        }
        const Instruction &currInstruction = it->second;

        std::string line = currInstruction.operatorBin;
        if ((currInstruction.operands.size() != instruction.size() - 1))
        {
            throw SyntaxError("Opcode: " + opCode + " Supports " + std::to_string(currInstruction.operands.size()) + " operands. " + std::to_string(instruction.size() - 1) + " operands were given.");
        }
    }

    return "Hello from assembler";
}