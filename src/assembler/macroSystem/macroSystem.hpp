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
    std::string evaluate(const std::string &block, size_t startingIndex = 0, size_t endingIndex = 0);

    void setNumber(std::string symbol, unsigned int value);
    void setMacro(std::string symbol, std::string value);
    void setLabel(std::string symbol, unsigned int value);
    Operand getVariable(std::string symbol);

    void pushStack();
    void popStack();

private:
    void _setVariableHelper(std::string symbol, Operand value, size_t stackIndex = 0);
    Operand _getVariableHelper(std::string symbol, size_t stackIndex = 0);

    void _handleAssignment(std::vector<std::string> &tokens, size_t &index);
    Operand _evaluateFunction(std::string function, std::vector<std::string> &tokens, size_t &index);
    Operand _handleEvaluation(std::vector<std::string> &tokens, size_t &index);
    Operand _parseOperand(std::vector<std::string> &tokens, size_t &index);
    
    size_t _currLineNum;

    std::vector<VariableMap> _variables;
    size_t _currentStack;
};