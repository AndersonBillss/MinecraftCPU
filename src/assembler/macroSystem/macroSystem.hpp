#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>
#include <set>


enum VariableType
{
    label,
    number,
    macro,
};
struct Variable
{
    VariableType type;
    std::variant<int, std::string> value;
};
using VariableMap = std::unordered_map<std::string, Variable>;

class MacroSystem
{
public:
    MacroSystem();
    std::string evaluate(const std::string &block);

    void setNumber(std::string symbol, int value);
    void setMacro(std::string symbol, std::string value);
    void setLabel(std::string symbol, int value);
    Variable getVariable(std::string symbol);

    void pushStack();
    void popStack();

private:
    void _setVariableHelper(std::string symbol, Variable value, size_t stackIndex = 0);
    Variable _getVariableHelper(std::string symbol, size_t stackIndex = 0);

    std::vector<std::string> _tokenize(std::string &block);
    int _handleBlock(std::vector<std::string> &tokens, std::string &text, int index);
    int _handleVariable(std::vector<std::string> &tokens, std::string &text, int index);

    std::vector<VariableMap> _variables;
    size_t _currentStack;
};