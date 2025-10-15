#pragma once
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "sourceLocation.hpp"
#include "lexer.hpp"

namespace Parser
{
    enum NodeType
    {
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
}