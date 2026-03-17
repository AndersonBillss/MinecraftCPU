#pragma once
#include <string>
#include <stack>
#include <unordered_map>
#include <vector>
#include <variant>
#include <set>
#include <functional>
#include "AST.hpp"
#include "operations.hpp"

class MacroSystem
{
public:
    using VariableMap = std::unordered_map<std::string, AST::Node *>;
    MacroSystem(AST::Node *node);

    std::string getLine();

    void setVariable(std::string symbol, AST::Node *value, size_t stackIndex = 0);
    AST::Node *getVariable(std::string symbol);

    void pushVariableStack();
    void popVariableStack();

    bool done();

private:
    struct AstLocation
    {
        AST::Node *node;
        size_t index;
    };

    std::stack<AstLocation> _callStack;

    AST::Node *_astNode;
    std::vector<VariableMap> _variables;
    size_t _currentStack;

    std::string _getLineHelper(AST::Node *node);
    void _pushCallStack(AST::Node *node);
    void _popCallStack();
    Operand _evaluateExpression(AST::Node *node);
    AST::Node *_getVariableHelper(std::string symbol, size_t stackIndex = 0);
    AST::Node *_getCurrNode();
    void _nextNode();
};