#include "tokenizer.hpp"
#include "../../utils/stringUtils.hpp"
#include <sstream>
#include <iostream>

void removeComments(std::string &line)
{
    if(line.empty()) return;
    for (int i = 0; i < line.size() - 1; i++)
    {
        if (line[i] == '/' && line[i + 1] == '/')
        {
            line.erase(i);
            break;
        }
    }
}

std::tuple<Tokenizer::SymbolMap, Tokenizer::InstructionList> Tokenizer::tokenize(const std::string &source)
{
    Tokenizer::SymbolMap symbols;
    Tokenizer::InstructionList instructions;

    std::istringstream stream(source);
    std::string line;
    
    int currLine = 0;
    while (std::getline(stream, line))
    {
        removeComments(line);
        std::string trimmed = stringUtils::trim(line);
        if(trimmed.empty()) continue;
        
        std::vector<std::string> splitLine = stringUtils::split(trimmed);
        if(splitLine[0][0] == '.'){
            symbols[splitLine[0]] = currLine;
            splitLine.erase(splitLine.begin());
        }
        
        instructions.push_back(splitLine);
        currLine++;
    }

    return std::make_tuple(symbols, instructions);
}