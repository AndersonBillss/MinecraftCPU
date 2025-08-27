#include "tokenizer.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/syntaxError.hpp"
#include <sstream>
#include <iostream>

Tokenizer::InstructionList Tokenizer::tokenize(const std::string &source)
{
    Tokenizer::InstructionList instructions;

    std::istringstream stream(source);
    std::string line;

    int currLine = 0;
    int fileLine = 0;
    while (std::getline(stream, line))
    {
        std::string trimmed = stringUtils::trim(line);

        fileLine++;
        if (trimmed.empty())
            continue;

        std::vector<std::string> splitLine = stringUtils::split(trimmed);
        if (splitLine.size() == 0)
            continue;

        instructions.push_back(
            {
                fileLineNumber : fileLine,
                tokens : splitLine,
            });
        currLine++;
    }

    return instructions;
}