#pragma once
#include <set>
#include <vector>
#include <iostream>

class AsmMacroLexer
{
    struct SourceLocation
    {
        size_t line;
        size_t column;
    };

public:
    std::vector<std::string> tokenize(const std::string &block, size_t startingIndex = 0, size_t endingIndex = 0);

private:
    void _advanceIndex(size_t n);
    SourceLocation _currentLocation;
    size_t _currLocation;
};