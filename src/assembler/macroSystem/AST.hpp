#pragma once

#include "sourceLocation.hpp"
#include <vector>
#include <memory>

namespace AST
{
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
    struct Node
    {
        SourceLocation begin;
        SourceLocation end;
        NodeType nodeType;

        std::vector<std::unique_ptr<Node>> children;

        std::string identifier;
        int intValue;
    };
}