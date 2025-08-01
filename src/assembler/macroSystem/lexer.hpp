#pragma once
#include <set>
#include <vector>
#include <iostream>

namespace AsmMacroLexer {
    extern std::set<char> operatorTokens;  
    std::vector<std::string> tokenize(const std::string &block);
}