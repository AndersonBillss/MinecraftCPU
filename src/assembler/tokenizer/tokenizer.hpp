#pragma once
#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>

namespace Tokenizer
{
    struct InstructionLine
    {
        int fileLineNumber; // This is used for more informative error messages
        std::vector<std::string> tokens;
    };
    using InstructionList = std::vector<InstructionLine>;

    InstructionList tokenize(const std::string &source);
}