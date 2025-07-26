#pragma once
#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>

namespace Tokenizer
{

    using SymbolMap = std::unordered_map<std::string, int>;
    using ConstMap = std::unordered_map<std::string, std::string>;
    struct InstructionLine
    {
        int fileLineNumber; // This is used for more informative error messages
        std::vector<std::string> tokens;
    };
    using InstructionList = std::vector<InstructionLine>;

    std::tuple<SymbolMap, ConstMap, InstructionList> tokenize(const std::string &source);
}