#include "tokenizer.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/syntaxError.hpp"
#include <sstream>
#include <iostream>

void removeComments(std::string &line)
{
    if (line.empty())
        return;
    for (size_t i = 0; i < line.size() - 1; i++)
    {
        if (line[i] == '#')
        {
            line.erase(i);
            break;
        }

        if (i + 1 >= line.size())
            break; // Avoid segfaults by breaking early
        if (line[i] == '/' && line[i + 1] == '/')
        {
            line.erase(i);
            break;
        }
    }
}

std::tuple<Tokenizer::SymbolMap, Tokenizer::ConstMap, Tokenizer::InstructionList> Tokenizer::tokenize(const std::string &source)
{
    Tokenizer::SymbolMap symbols;
    Tokenizer::ConstMap constants;
    Tokenizer::InstructionList instructions;

    std::istringstream stream(source);
    std::string line;

    int currLine = 0;
    int fileLine = 0;
    while (std::getline(stream, line))
    {
        removeComments(line);
        std::string trimmed = stringUtils::trim(line);

        fileLine++;
        if (trimmed.empty())
            continue;

        std::vector<std::string> splitLine = stringUtils::split(trimmed);
        if (splitLine[0][0] == '.')
        {
            symbols[splitLine[0]] = currLine;
            splitLine.erase(splitLine.begin());
        }

        if (splitLine[0][0] == '$')
        {
            std::vector<std::string> equalSplit = stringUtils::split(trimmed, "=");
            std::string label = stringUtils::trim(equalSplit[0]);
            std::string value = stringUtils::trim(equalSplit[1]);
            constants[label] = value;
            continue;
        }

        if (splitLine.size() == 0)
            continue;

        instructions.push_back(
            {
                fileLineNumber : fileLine,
                tokens : splitLine,
            });
        currLine++;
    }

    return std::make_tuple(symbols, constants, instructions);
}