#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <set>
#include <functional>

enum OldVariableType
{
    number,
    macro,
};

using VariableMap = std::unordered_map<std::string, std::variant<unsigned int, std::string>>;

class OldMacroSystem
{
public:
    OldMacroSystem();
    std::string evaluate(const std::string &block);
    std::string replaceLocationSymbols(const std::string &block);

    void setNumber(std::string symbol, unsigned int value);
    void setString(std::string symbol, std::string value);
    void setLabel(std::string symbol, unsigned int value);
    std::variant<unsigned int, std::string> getVariable(std::string symbol);

    void pushStack();
    void popStack();

private:
    void _setVariableHelper(std::string symbol, std::variant<unsigned int, std::string> value, size_t stackIndex = 0);
    std::variant<unsigned int, std::string> _getVariableHelper(std::string symbol, size_t stackIndex = 0);

    void _handleAssignment(std::vector<std::string> &tokens, size_t &index);
    std::variant<unsigned int, std::string> _evaluateFunction(std::string function, std::vector<std::string> &tokens, size_t &index);
    std::variant<unsigned int, std::string> _handleEvaluation(std::vector<std::string> &tokens, size_t &index);
    std::variant<unsigned int, std::string> _parseOperand(std::vector<std::string> &tokens, size_t &index);
    std::string _evaluateHelper(const std::string &block, size_t startingIndex, size_t endingIndex);

    std::vector<VariableMap> _variables;
    size_t _currentStack;
};