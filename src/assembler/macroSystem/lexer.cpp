#include <cctype>
#include "lexer.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/runtimeError.hpp"
#include "newLexer.hpp"

std::set<char> operatorTokens = {
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
};

int handleOperator(std::vector<std::string> &tokens, const std::string &text, size_t index)
{
    std::string fullOperator = "";
    while (index < text.size())
    {
        if (
            (std::isspace(text[index])) ||
            (index == text.size() - 1) ||
            (operatorTokens.find(text[index]) == operatorTokens.end()))
        {
            tokens.push_back(fullOperator);
            return fullOperator.size();
        }
        fullOperator += text[index];
        index++;
    }
    tokens.push_back(fullOperator);
    return fullOperator.size();
}

int handleFullWord(std::vector<std::string> &tokens, const std::string &text, size_t index, size_t endingIndex)
{
    std::string fullWord = "";
    while (index < endingIndex)
    {
        if (
            (std::isspace(text[index])) ||
            (index >= endingIndex) ||
            (operatorTokens.find(text[index]) != operatorTokens.end()))
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

int handleBlock(std::vector<std::string> &tokens, const std::string &text, size_t index)
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

int handleComment(std::vector<std::string> &tokens, const std::string &text, size_t index)
{
    for (size_t i = index; i < text.size(); i++)
    {
        if (text[i] == '\n')
        {
            return i - index;
        }
    }
    return text.size();
}

int handleSymbol(std::vector<std::string> &tokens, const std::string &text, size_t index)
{
    std::string symbol = "";
    size_t i = index;
    while (i < text.size() && !std::isspace(text[i]) && text[i] != '#' && !stringUtils::subSectionEqual(text, i, "//"))
    {
        symbol += text[i];
        i++;
    }
    tokens.push_back(symbol);
    return symbol.size();
}

std::vector<std::string> AsmMacroLexer::tokenize(const std::string &block, size_t startingIndex, size_t endingIndex)
{
    size_t index = startingIndex;
    size_t end = endingIndex;
    if (endingIndex <= startingIndex)
    {
        end = block.size();
    }
    std::vector<std::string> tokens;
    while (index < end)
    {
        if (block[index] == '#')
        {
            index += handleComment(tokens, block, index);
        }
        else if (stringUtils::subSectionEqual(block, index, "//"))
        {
            index += handleComment(tokens, block, index);
        }
        else if (block[index] == '.')
        {
            index += handleSymbol(tokens, block, index);
        }
        else if (operatorTokens.find(block[index]) != operatorTokens.end())
        {
            index += handleOperator(tokens, block, index);
        }
        else if (block[index] == '(')
        {
            index += handleBlock(tokens, block, index);
        }
        else if (block[index] == '\n')
        {
            if (tokens.size() == 0)
            {
                index++;
            }
            else if (tokens[tokens.size() - 1] == "\n")
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
            index += handleFullWord(tokens, block, index, end);
        }
    }
    if (tokens[tokens.size() - 1] == "\n")
    {
        tokens.pop_back();
    }
    return tokens;
}

void AsmMacroLexer::_advanceIndex(size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        size_t index = i + _currIndex;
    }
}
