#include <cctype>
#include "lexer.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/runtimeError.hpp"

std::set<char> AsmMacroLexer::operatorTokens = {
    '+',
    '-',
    '*',
    '/',
    '=',
    '|',
    '&',
    '^',
    '<',
    '>',
    ',',
    '.',
};

int handleOperator(std::vector<std::string> &tokens, std::string &text, size_t index)
{
    std::string fullOperator = "";
    while (index < text.size())
    {
        index++;
        if (
            (std::isspace(text[index])) ||
            (index == text.size() - 1) ||
            (AsmMacroLexer::operatorTokens.find(text[index]) == AsmMacroLexer::operatorTokens.end()))
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
        if (
            (std::isspace(text[index])) ||
            (index == text.size() - 1) ||
            (AsmMacroLexer::operatorTokens.find(text[index]) != AsmMacroLexer::operatorTokens.end()))
        {
            tokens.push_back(fullWord);
            return fullWord.size();
        }
        fullWord += text[index];
        index++;
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
    catch (std::runtime_error &)
    {
        throw RuntimeError("Failed to find matching bracket for block");
    }
    tokens.push_back(subBlock);
    return subBlock.size();
}

std::vector<std::string> AsmMacroLexer::tokenize(std::string &block)
{
    std::vector<std::string> tokens;
    size_t index = 0;
    while (index < block.size())
    {
        if (block[index] == '(')
        {
            index += handleBlock(tokens, block, index);
        }
        else if (AsmMacroLexer::operatorTokens.find(block[index]) != AsmMacroLexer::operatorTokens.end())
        {
            index += handleOperator(tokens, block, index);
        }
        else if (block[index] == '\n')
        {
            if (tokens.size() == 0)
            {
                index++;
            }
            else if (tokens[index - 1] == "\n")
            {
                index++;
            }
            else
            {
                tokens.push_back("\n");
                index++;
            }
        }
        else if (std::isspace(block[index]))
        {
            index++;
        }
        else
        {
            index += handleFullWord(tokens, block, index);
            index++;
        }
    }
    if (tokens[tokens.size() - 1] == "\n")
    {
        tokens.pop_back();
    }
    return tokens;
}
