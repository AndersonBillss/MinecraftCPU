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
    AST parseTokens(std::vector<AsmMacroLexer::Token> &tokens);

private:
    Parser::AST _tree;
    size_t _currIndex;
    std::vector<AsmMacroLexer::Token> _tokens;

    bool _isFunction();
    bool _isAssignment();
    void _handleFunction();
    void _handleAssignment();
    void _handleParentheses();
    void _handleFirstOperand();
    NodeType _handleOpType();
    void _handleExpression();
    void _handleLine();

public:
    Parser()
    {
        _currIndex = 0;
        _tokens = {};
        _tree = Parser::AST{
            _tokens[_currIndex + 2].begin,
            {0, 0},
            Parser::NodeType::PROGRAM,
            {},
            "",
            0};
    }
};
