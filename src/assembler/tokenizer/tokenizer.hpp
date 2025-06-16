#pragma once
#include <unordered_map>
#include <vector>
#include <tuple>
#include <string>

namespace Tokenizer
{
    struct InstructionLine
    {
        int fileLineNumber; // This is used for more informative error messages
        std::vector<std::string> tokens;
    };
    using SymbolMap = std::unordered_map<std::string, int>;
    using InstructionList = std::vector<InstructionLine>;

    std::tuple<SymbolMap, InstructionList> tokenize(const std::string &source);
}