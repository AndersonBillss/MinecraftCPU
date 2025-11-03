#pragma once
#include <string>
#include <vector>
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
        RETURN,

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
    std::unique_ptr<Parser::AST> parseTokens(std::vector<AsmMacroLexer::Token> &tokens);

private:
    std::unique_ptr<Parser::AST> _root;
    size_t _currIndex;
    std::vector<AsmMacroLexer::Token> _tokens;
    std::unordered_map<std::string, std::unique_ptr<AST>> _functions;

    bool _isFunction();
    bool _isAssignment();
    NodeType _handleOpType();
    std::unique_ptr<Parser::AST> _handleFunction();
    std::unique_ptr<Parser::AST> _handleAssignment();
    std::unique_ptr<Parser::AST> _handleParentheses();
    std::unique_ptr<Parser::AST> _handleOperand();
    std::unique_ptr<Parser::AST> _handleExpression(int previousNodePrecedence = -1);
    std::unique_ptr<Parser::AST> _handleLine();

public:
    Parser()
    {
        _currIndex = 0;
        _tokens = {};
    }
};
