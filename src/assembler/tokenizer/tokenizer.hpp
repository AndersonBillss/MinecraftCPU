#pragma once
#include <unordered_map>
#include <vector>
#include <tuple>
#include <string>

namespace Tokenizer
{
    using SymbolMap = std::unordered_map<std::string, int>;
    using InstructionList = std::vector<std::vector<std::string>>;

    std::tuple<SymbolMap, InstructionList> tokenize(const std::string &source);
}