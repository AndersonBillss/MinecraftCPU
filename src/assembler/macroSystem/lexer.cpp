#include <cctype>
#include "lexer.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/runtimeError.hpp"

int handleOperator(std::vector<std::string> &tokens, std::string &text, size_t index)
{
    std::string fullOperator = "";
    while (index < text.size())
    {
        index++;
        if (
            (std::isspace(text[index])) ||
            (index == text.size() - 1) ||
            (operatorTokens.find(text[index]) == operatorTokens.end()))
        {
            tokens.push_back(fullOperator);
            return fullOperator.size();
        }
        fullOperator += text[index];
    }
    tokens.push_back(fullOperator);
    return fullOperator.size();
}

int handleFullWord(std::vector<std::string> &tokens, std::string &text, size_t index)
{
    std::string fullWord = "";
    while (index < text.size())
    {
        index++;
        if (
            (std::isspace(text[index])) ||
            (index == text.size() - 1) ||
            (operatorTokens.find(text[index]) != operatorTokens.end()))
        {
            tokens.push_back(fullWord);
            return fullWord.size();
        }
        fullWord += text[index];
    }
    tokens.push_back(fullWord);
    return fullWord.size();
}

int handleBlock(std::vector<std::string> &tokens, std::string &text, size_t index)
{
    std::string subBlock;
    try
    {
        subBlock = stringUtils::getBlock(text, "(", ")", index);
    }
    catch (std::runtime_error&)
    {
        throw RuntimeError("Failed to find matching bracket for block");
    }
    tokens.push_back(subBlock);
    return subBlock.size();
}


std::vector<std::string> tokenize(std::string &block)
{
    std::vector<std::string> tokens;
    size_t index = 0;
    while (index < block.size())
    {
        if (block[index] == '(')
        {
            index += handleBlock(tokens, block, index);
        }
        else if (operatorTokens.find(block[index]) != operatorTokens.end())
        {
            index += handleOperator(tokens, block, index);
        }
        else if (block[index] == '\n')
        {
            tokens.push_back("\n");
            index++;
        }
        else if (std::isspace(block[index]))
        {
            index++;
        }
        else
        {
            index += handleFullWord(tokens, block, index);
        }
    }
    return tokens;
}
