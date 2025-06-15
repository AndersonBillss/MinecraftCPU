#include "assembler.hpp"
#include "tokenizer/tokenizer.hpp"

#include <iostream>

std::string Assembler::compile(const std::string &sourceCode)
{
    
    std::tuple<Tokenizer::SymbolMap, Tokenizer::InstructionList> tokenized = Tokenizer::tokenize(sourceCode);
    Tokenizer::SymbolMap symbols = std::get<0>(tokenized);
    Tokenizer::InstructionList instructions = std::get<1>(tokenized);

    for (int i = 0; i < instructions.size(); i++)
    {
        for (int j = 0; j < instructions[i].size(); j++)
        {
            std::cout << instructions[i][j] << " ";
        }
        std::cout << std::endl;
    }


    return "Hello from assembler";
}