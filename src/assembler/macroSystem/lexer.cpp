#include <cctype>
#include "lexer.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/runtimeError.hpp"

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
};

int handleOperator(std::vector<std::string> &tokens, const std::string &text, size_t index)
{
    std::string fullOperator = "";
    while (index < text.size())
    {
        if (
            (std::isspace(text[index])) ||
            (index == text.size() - 1) ||
            (AsmMacroLexer::operatorTokens.find(text[index]) == AsmMacroLexer::operatorTokens.end()))
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
        if (AsmMacroLexer::operatorTokens.find(block[index]) != AsmMacroLexer::operatorTokens.end())
        {
            index += handleOperator(tokens, block, index);
        }
        else if (block[index] == '#')
        {
            index += handleComment(tokens, block, index);
        }
        else if (block[index] == '/')
        {
            if (index + 1 >= block.size())
            {
                index += 1;
            }
            else if (block[index + 1] == '/')
            {
                index += handleComment(tokens, block, index);
            }
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
