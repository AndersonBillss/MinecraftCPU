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

void AsmMacroLexer::_advanceIndex(size_t n)
{
    for (size_t i = 0; i < n; i++)
    {
        if (_currIndex >= _endIndex)
        {
            return;
        }
        _currIndex++;
        _currLocation.column++;
        if (_sourceCode[_currIndex] == '\n')
        {
            _currLocation.column = 0;
            _currLocation.line++;
        }
    }
}

void AsmMacroLexer::_pushToken(std::string data, TokenType type)
{
    SourceLocation begin = _currLocation;
    _advanceIndex(data.size() - 1);
    _tokens.push_back({
        begin : begin,
        end : _currLocation,
        type : type,
        data : data
    });
    _advanceIndex(1);
}

void AsmMacroLexer::_handleOperator()
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
        i++;
    }
    _pushToken(fullOperator, TokenType::OPERATOR);
}

void AsmMacroLexer::_handleFullWord()
{
    std::string fullWord = "";
    size_t i = _currIndex;
    while (i < _endIndex)
    {
        if (
            (std::isspace(_sourceCode[i])) ||
            (i >= _endIndex) ||
            (operatorTokens.find(_sourceCode[i]) != operatorTokens.end()))
        {
            TokenType t = _sourceCode[i] == '$' ? TokenType::SYMBOL : TokenType::VALUE;
            _pushToken(fullWord, t);
            return;
        }
        fullWord += _sourceCode[i];
        i++;
    }
    TokenType t = _sourceCode[i] == '$' ? TokenType::SYMBOL : TokenType::VALUE;
    _pushToken(fullWord, t);
}

void AsmMacroLexer::_handleLocationMarker()
{
    std::string fullWord = "";
    size_t i = _currIndex;
    while (i < _endIndex)
    {
        if (
            (std::isspace(_sourceCode[i])) ||
            (i >= _endIndex))
        {
            TokenType t = _sourceCode[i] == '$' ? TokenType::SYMBOL : TokenType::VALUE;
            _pushToken(fullWord, t);
        }
        fullWord += _sourceCode[i];
        i++;
    }
    TokenType t = _sourceCode[i] == '$' ? TokenType::SYMBOL : TokenType::VALUE;
    _pushToken(fullWord, t);
}

void AsmMacroLexer::_handleComment()
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

void AsmMacroLexer::_handleNewLine()
{
    _currLocation.line++;
    if (_tokens.size() == 0)
    {
        _currIndex++;
    }
    else if (_tokens[_tokens.size() - 1].type == TokenType::ENDLINE)
    {
        _currIndex++;
    }
    else
    {
        _pushToken("\n", TokenType::ENDLINE);
    }
}

std::vector<AsmMacroLexer::Token> AsmMacroLexer::tokenize(const std::string &block)
{
    _currIndex = 0;
    _currLocation = {
        line : 0,
        column : 0
    };
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
        else if (_sourceCode[_currIndex] == '.')
        {
            _handleLocationMarker();
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
