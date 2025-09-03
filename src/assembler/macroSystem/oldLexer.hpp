#pragma once
#include <set>
#include <vector>
#include <iostream>

namespace OldAsmMacroLexer {
    extern std::set<char> operatorTokens;  
    std::vector<std::string> tokenize(const std::string &block, size_t startingIndex = 0, size_t endingIndex = 0);
}