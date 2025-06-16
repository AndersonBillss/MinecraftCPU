#include "assembler.hpp"
#include "tokenizer/tokenizer.hpp"
#include "instruction.hpp"
#include "../utils/syntaxError.hpp"
#include "../utils/stringUtils.hpp"

#include <iostream>
#include <iterator>

std::string Assembler::compile(const std::string &sourceCode)
{
    std::tuple<Tokenizer::SymbolMap, Tokenizer::InstructionList> tokenized = Tokenizer::tokenize(sourceCode);
    Tokenizer::SymbolMap symbols = std::get<0>(tokenized);
    Tokenizer::InstructionList instructions = std::get<1>(tokenized);

    std::vector<std::string> compiled;

    for (std::vector<std::string> instruction : instructions)
    {
        std::string opCode = instruction[0];
        std::vector<std::string> operands(instruction.begin() + 1, instruction.end());

        auto it = instructionTable.find(opCode);
        if (it == instructionTable.end())
        {
            throw SyntaxError("Opcode " + opCode + " does not exist.");
        }
        const Instruction &currInstruction = it->second;

        std::vector<std::string> line = {currInstruction.operatorBin};
        if ((currInstruction.operands.size() != operands.size()))
        {
            throw SyntaxError("Opcode " + opCode + " Supports " + std::to_string(currInstruction.operands.size()) + " operand(s). " + std::to_string(operands.size()) + " operand(s) were given.");
        }

        for (int i = 0; i < operands.size(); i++)
        {
            std::string operand = operands[i];
            if (operand[0] == '.')
            {
                auto it = symbols.find(operand);
                if (it == symbols.end())
                {
                    throw SyntaxError("Symbol " + operand + " does not exist.");
                }
                operand = std::to_string(symbols[operand]);
            }
            std::string opBin = currInstruction.operands[i]->toBin(operand);
            line.push_back(opBin);
        }

        while(line.size() < 3) {
            line.push_back("00000000");
        }
        
        compiled.push_back(stringUtils::join(line, " "));
    }

    return stringUtils::join(compiled, "\n");
}