#include "lexer.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/runtimeError.hpp"

std::vector<std::string> tokenize(std::string &block)
{
    std::vector<std::string> tokens;
    size_t index;
    while (index < block.size())
    {
        if (block[index] == '{')
        {
            index += handleBlock(tokens, block, index);
        }
        if (block[index] == '$')
        {
        }
    }
}

int handleBlock(std::vector<std::string> &tokens, std::string &text, int index)
{
    std::string subBlock;
    try
    {
        subBlock = stringUtils::getBlock(text, "{", "}", index);
    }
    catch (std::runtime_error)
    {
        throw RuntimeError("Failed to find matching bracket for block");
    }
    tokens.push_back(subBlock);
    return subBlock.size();
}