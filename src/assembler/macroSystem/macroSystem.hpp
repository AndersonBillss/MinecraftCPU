#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <set>
#include <functional>
#include "AST.hpp"

class MacroSystem
{
public:
    using VariableMap = std::unordered_map<std::string, AST::Node *>;
    MacroSystem();
    std::string evaluate(const std::string &block);

    void setVariable(std::string symbol, AST::Node *value, size_t stackIndex = 0);
    AST::Node *getVariable(std::string symbol);

    void pushStack();
    void popStack();

private:
    std::vector<VariableMap> _variables;
    size_t _currentStack;

    AST::Node *_getVariableHelper(std::string symbol, size_t stackIndex = 0);
};