#pragma once
#include <set>
#include <vector>
#include <iostream>

class AsmMacroLexer
{
public:
    struct SourceLocation
    {
        size_t line;
        size_t column;
    };
    enum TokenType
    {
        OPERATOR,
        SYMBOL,
        VALUE,
        LOCATIONMARKER,
        OPENINGPARENTHESE,
        CLOSINGPARENTHESE,
        ENDLINE
    };
    struct Token
    {
        SourceLocation begin;
        SourceLocation end;
        TokenType type;
        std::string data;
    };
    std::vector<Token> tokenize(const std::string &block);

private:
    void _advanceIndex(size_t n);
    void _pushToken(std::string data, TokenType type);

    std::vector<Token> _tokens;
    std::string _sourceCode;
    SourceLocation _currLocation;
    size_t _currIndex;
    size_t _endIndex;

    void _handleOperator();
    void _handleComment();
    void _handleLocationMarker();
    void _handleFullWord();
    void _handleNewLine();
};