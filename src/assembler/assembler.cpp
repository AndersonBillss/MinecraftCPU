#include "assembler.hpp"
#include "tokenizer/tokenizer.hpp"
#include "instruction.hpp"
#include "../utils/syntaxError.hpp"
#include "../utils/stringUtils.hpp"

#include <iostream>
#include <iterator>

std::string Assembler::compile(const std::string &sourceCode)
{
    std::tuple<Tokenizer::SymbolMap, Tokenizer::ConstMap, Tokenizer::InstructionList> tokenized = Tokenizer::tokenize(sourceCode);
    Tokenizer::SymbolMap symbols = std::get<0>(tokenized);
    Tokenizer::ConstMap constants = std::get<1>(tokenized);
    Tokenizer::InstructionList instructions = std::get<2>(tokenized);

    std::vector<std::string> compiled;

    for (Tokenizer::InstructionLine instruction : instructions)
    {
        std::string opCode = instruction.tokens[0];
        std::vector<std::string> operands(instruction.tokens.begin() + 1, instruction.tokens.end());

        auto it = instructionTable.find(opCode);
        if (it == instructionTable.end())
        {
            throw SyntaxError("(line " + std::to_string(instruction.fileLineNumber) + ") Opcode " + opCode + " does not exist.");
        }
        const Instruction &currInstruction = it->second;

        std::vector<std::string> line = {currInstruction.operatorBin};
        if ((currInstruction.operands.size() != operands.size()))
        {
            throw SyntaxError("(line " + std::to_string(instruction.fileLineNumber) + ") Opcode '" + opCode + "' Supports " + std::to_string(currInstruction.operands.size()) + " operand(s). " + std::to_string(operands.size()) + " operand(s) were given.");
        }

        for (size_t i = 0; i < operands.size(); i++)
        {
            std::string operand = operands[i];
            if (operand[0] == '.')
            {
                auto it = symbols.find(operand);
                if (it == symbols.end())
                {
                    throw SyntaxError("(line " + std::to_string(instruction.fileLineNumber) + ") Symbol '" + operand + "' does not exist.");
                }
                operand = std::to_string(symbols[operand]);
            }
            if (operand[0] == ':')
            {
                auto it = constants.find(operand);
                if (it == constants.end())
                {
                    throw SyntaxError("(line " + std::to_string(instruction.fileLineNumber) + ") constant '" + operand + "' does not exist.");
                }
                operand = std::to_string(symbols[operand]);
            }

            std::string opBin;
            try
            {
                opBin = currInstruction.operands[i]->toBin(operand);
            }
            catch (const SyntaxError &e)
            {
                throw SyntaxError("(line " + std::to_string(instruction.fileLineNumber) + ") " + e.what());
            }
            line.push_back(opBin);
        }

        while (line.size() < 3)
        {
            line.push_back("00000000");
        }

        if (opCode == "STR" || opCode == "STR_PTR")
        { // In hardware, the opcodes are switched for these two
            line = {line[0], line[2], line[1]};
        }

        compiled.push_back(stringUtils::join(line, " "));
    }

    return stringUtils::join(compiled, "\n");
}