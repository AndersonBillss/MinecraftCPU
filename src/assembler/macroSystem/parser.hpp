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
        NEWLINE,
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
        PARAMETER,
        RETURN,

        IDENTIFIER,
        STRING,
        INT,
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

    class ParseError : public std::exception
    {
        std::string message;
    public:
        ParseError(const std::string &msg) : message(msg) {}
        const char *what() const noexcept override { return message.c_str(); }
    };
}