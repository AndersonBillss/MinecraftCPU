#include <cctype>
#include "lexer.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/runtimeError.hpp"

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

void Lexer::_advanceIndex(size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        _currLocation.column++;
        if (_sourceCode[_currIndex] == '\n')
        {
            _currLocation.column = 0;
            _currLocation.line++;
        }
        _currIndex++;
        if (_currIndex >= _endIndex)
        {
            return;
        }
    }
}

void Lexer::_pushToken(std::string data, TokenType type)
{
    SourceLocation begin = _currLocation;
    _advanceIndex(data.size() - 1);
    _tokens.push_back({begin,
                       _currLocation,
                       type,
                       data});
    _advanceIndex(1);
}

void Lexer::_handleOperator()
{
    std::string fullOperator = "";
    size_t i = _currIndex;
    while (i < _endIndex)
    {
        if (
            (std::isspace(_sourceCode[i])) ||
            (i == _endIndex - 1) ||
            (operatorTokens.find(_sourceCode[i]) == operatorTokens.end()))
        {
            _pushToken(fullOperator, TokenType::OPERATOR);
            return;
        }
        fullOperator += _sourceCode[i];
        i++;
    }
    _pushToken(fullOperator, TokenType::OPERATOR);
}

void Lexer::_handleFullWord()
{
    Lexer::TokenType type = _sourceCode[_currIndex] == '$' ? TokenType::SYMBOL : TokenType::VALUE;
    std::string fullWord = "";
    size_t i = _currIndex;
    while (i < _endIndex)
    {
        if (
            (std::isspace(_sourceCode[i])) ||
            (i >= _endIndex) ||
            (operatorTokens.find(_sourceCode[i]) != operatorTokens.end()) ||
            (_sourceCode[i] == ')') ||
            (_sourceCode[i] == '('))
        {
            _pushToken(fullWord, type);
            return;
        }
        fullWord += _sourceCode[i];
        i++;
    }
    _pushToken(fullWord, type);
}

void Lexer::_handleComment()
{
    for (size_t i = _currIndex; i < _endIndex; i++)
    {
        if (_sourceCode[i] == '\n')
        {
            _advanceIndex(1);
            return;
        }
    }
}

void Lexer::_handleNewLine()
{
    if (_tokens.size() == 0)
    {
        _advanceIndex(1);
    }
    else if (_tokens[_tokens.size() - 1].type == TokenType::ENDLINE)
    {
        _advanceIndex(1);
    }
    else
    {
        _pushToken("\n", TokenType::ENDLINE);
    }
}

std::vector<Lexer::Token> Lexer::tokenize(const std::string &block)
{
    _currIndex = 0;
    _currLocation = {
        0,
        0};
    _endIndex = block.size();
    _sourceCode = block;
    while (_currIndex < _endIndex)
    {
        if (_sourceCode[_currIndex] == '#')
        {
            _handleComment();
        }
        else if (stringUtils::subSectionEqual(_sourceCode, _currIndex, "//"))
        {
            _handleComment();
        }
        else if (operatorTokens.find(_sourceCode[_currIndex]) != operatorTokens.end())
        {
            _handleOperator();
        }
        else if (_sourceCode[_currIndex] == '(')
        {
            _pushToken("(", TokenType::OPENINGPARENTHESE);
        }
        else if (_sourceCode[_currIndex] == ')')
        {
            _pushToken(")", TokenType::CLOSINGPARENTHESE);
        }
        else if (_sourceCode[_currIndex] == '\n')
        {
            _handleNewLine();
        }
        else if (std::isspace(_sourceCode[_currIndex]))
        {
            _advanceIndex(1);
        }
        else
        {
            _handleFullWord();
        }
    }
    if (_tokens[_tokens.size() - 1].type == TokenType::ENDLINE)
    {
        _tokens.pop_back();
    }
    return _tokens;
}
