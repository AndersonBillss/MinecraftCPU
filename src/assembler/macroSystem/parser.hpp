#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include <iostream>
#include "sourceLocation.hpp"
#include "lexer.hpp"

class Parser
{
public:
    enum NodeType
    {
        PROGRAM,
        BLOCK,
        LINE,

        ADD,
        SUB,
        OR,
        AND,
        XOR,
        MULTIPLY,
        DIVIDE,
        CONCAT,

        ASSIGNMENT,
        FUNCTION,
        PARAMETER_LIST,
        CALL,
        ARGUMENT_LIST,

        IDENTIFIER,
        STRING,
        INT,
        UNDEFINED,
    };

    struct AST
    {
        SourceLocation begin;
        SourceLocation end;
        NodeType nodeType;

        std::vector<std::unique_ptr<AST>> children;

        std::string identifier;
        int intValue;
    };
    std::unique_ptr<Parser::AST> parseTokens(std::vector<Lexer::Token> &tokens);


private:
    std::unique_ptr<Parser::AST> _root;
    size_t _currIndex;
    std::vector<Lexer::Token> _tokens;
    std::unordered_map<std::string, AST*> _functions;

    bool _isFunction();
    bool _isAssignment();
    bool _functionExists(std::string functionName);
    NodeType _handleOpType();
    std::unique_ptr<Parser::AST> _handleFunction(std::string functionName);
    bool _isFunctionCallEndingNonterminal();
    std::unique_ptr<Parser::AST> _handleFunctionCall();
    std::unique_ptr<Parser::AST> _handleAssignment();
    std::unique_ptr<Parser::AST> _handleParentheses();
    std::unique_ptr<Parser::AST> _parseAtom();
    bool _isExpressionEndingNonterminal();
    std::unique_ptr<Parser::AST> _handleExpressionHelper(std::unique_ptr<Parser::AST> lhs, int minPrecedence);
    std::unique_ptr<Parser::AST> _handleExpression();
    std::unique_ptr<Parser::AST> _handleLine();

public:
    Parser()
    {
        _currIndex = 0;
        _tokens = {};
    }
};
