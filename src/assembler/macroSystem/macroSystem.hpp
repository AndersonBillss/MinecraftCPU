#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <set>
#include <functional>

enum VariableType
{
    number,
    macro,
};

using Operand = std::variant<unsigned int, std::string>; 
using VariableMap = std::unordered_map<std::string, Operand>;

class MacroSystem
{
public:
    MacroSystem();
    std::string evaluate(const std::string &block);
    std::string replaceLocationSymbols(const std::string &block);

    void setNumber(std::string symbol, unsigned int value);
    void setString(std::string symbol, std::string value);
    void setLabel(std::string symbol, unsigned int value);
    Operand getVariable(std::string symbol);

    void pushStack();
    void popStack();
};