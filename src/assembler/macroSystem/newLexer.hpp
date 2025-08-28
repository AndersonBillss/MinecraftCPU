#pragma once
#include <set>
#include <vector>
#include <iostream>

class newAsmMacroLexer
{
public:
    enum TokenType
    {
        Identifier,
        Symbol,
        Int,
        Text,
        Operator,
        Equal,
        LeftParenthese,
        RightParenthese,
        NewLine
    };
    struct SourceLocation
    {
        size_t line;
        size_t column;
    };
    struct Token
    {
        TokenType type;
        std::string text;
        SourceLocation start;
        SourceLocation end;
    };
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
    std::vector<Token> tokenize(const std::string &block, size_t startingIndex = 0, size_t endingIndex = 0);

private:
    SourceLocation currentLocation;
    size_t currentCharacter;

    void _moveLocation(size_t offset);
    void _addToken(size_t offset, TokenType type);

    void _moveLocation(std::string subSection);
    void _untilLocationInclude(std::set<char> characterTypes);
    void _untilLocationExclude(std::set<char> characterTypes);

    size_t _until(std::string subSection);
};