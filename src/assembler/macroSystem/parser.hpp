#include <string>
#include <vector>
#include <memory>
#include "sourceLocation.hpp"

enum NodeType {
    NEWLINE,
    BLOCK,

    ADD,
    SUB,
    OR,
    AND,
    XOR,
    EQUAL,
    MULTIPLY,
    DIVIDE,

    PARAMETER,
    RETURN,

    VARIABLE,
    STRING,
    INT,
};

struct AST {
    SourceLocation begin;
    SourceLocation end;
    NodeType nodeType;

    std::vector<std::unique_ptr<AST>> children;

    std::string identifier;   
    int intValue;
};
