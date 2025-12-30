#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <set>
#include <functional>
#include "parser.hpp"

class MacroSystem
{
public:
    using VariableMap = std::unordered_map<std::string, Parser::AST *>;
    MacroSystem();
    std::string evaluate(const std::string &block);

    void setVariable(std::string symbol, Parser::AST *value, size_t stackIndex = 0);
    Parser::AST *getVariable(std::string symbol);

    void pushStack();
    void popStack();

private:
    std::vector<VariableMap> _variables;
    size_t _currentStack;

    Parser::AST *_getVariableHelper(std::string symbol, size_t stackIndex = 0);
};