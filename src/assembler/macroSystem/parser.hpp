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
    Parser::AST _root;
    size_t _currIndex;
    std::vector<AsmMacroLexer::Token> _tokens;

    bool _isFunction();
    bool _isAssignment();
    void _handleFunction(Parser::AST &tree);
    void _handleAssignment(Parser::AST &tree);
    std::unique_ptr<Parser::AST> _handleParentheses(Parser::AST &tree);
    std::unique_ptr<Parser::AST> _handleOperand(Parser::AST &tree);
    NodeType _handleOpType();
    std::unique_ptr<Parser::AST> _handleExpression(Parser::AST &tree, int previousNodePrecedence = -1);
    void _handleLine(Parser::AST &tree);

public:
    Parser()
    {
        _currIndex = 0;
        _tokens = {};
        _root = Parser::AST{
            _tokens[_currIndex + 2].begin,
            {0, 0},
            Parser::NodeType::PROGRAM,
            {},
            "",
            0};
    }
};
