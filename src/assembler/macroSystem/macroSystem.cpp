#include "macroSystem.hpp"
#include "../../utils/runtimeError.hpp"
#include "../../utils/stringUtils.hpp"
#include "./lexer.hpp"

MacroSystem::MacroSystem()
{
    _currentStack = -1; // pushStack automatically increments this
    pushStack();
}

void MacroSystem::setNumber(std::string symbol, unsigned int value)
{
    _setVariableHelper(symbol, value, _currentStack);
}

void MacroSystem::setMacro(std::string symbol, std::string value)
{
    _setVariableHelper(symbol, value, _currentStack);
}

void MacroSystem::setLabel(std::string symbol, unsigned int value)
{
    _setVariableHelper(symbol, value);
}

void MacroSystem::pushStack()
{
    VariableMap variables;
    _variables.push_back(variables);
    _currentStack++;
}

void MacroSystem::popStack()
{
    if (_currentStack <= 0)
    {
        throw RuntimeError("Cannot pop a stack that doesn't exist");
    }
    _variables.pop_back();
    _currentStack--;
}

Operand MacroSystem::getVariable(std::string symbol)
{
    return _getVariableHelper(symbol, _currentStack);
}

Operand MacroSystem::_getVariableHelper(std::string symbol, size_t stackIndex)
{
    VariableMap &scope = _variables[stackIndex];
    auto it = scope.find(symbol);
    if (it != scope.end())
    {
        return it->second;
    }
    else
    {
        if (stackIndex == 0)
        {
            throw RuntimeError("Variable not found: '" + symbol + "'");
        }
        return _getVariableHelper(symbol, 0);
    }
}

void MacroSystem::_setVariableHelper(std::string symbol, Operand value, size_t stackIndex)
{
    VariableMap &scope = _variables[0];

    auto it = scope.find(symbol);
    if (it != scope.end())
    {
        scope[symbol] = value;
    }
    else
    {
        _variables[_currentStack][symbol] = value;
    }
}

std::string MacroSystem::evaluate(const std::string &block)
{
    std::vector<std::string> tokens = AsmMacroLexer::tokenize(block);
    size_t index = 0;
    while (index < block.size())
    {
        if (tokens[index][0] == '.')
        {
            setLabel(tokens[index], _currLineNum);
            index++;
        }
        else if ((index + 1 >= tokens.size()) && (tokens[index + 1] == "="))
        {
            _handleAssignment(tokens, index);
        }
        else if (tokens[index] == "\n")
        {
            index++;
        }
        else
        {
            _handleEvaluation(tokens, index);
        }
    }
    return "";
}

void MacroSystem::_handleAssignment(std::vector<std::string> &tokens, size_t &index)
{
    std::string variableKey;
    index += 2; // Move the index past the equal sign
    Operand variableValue = _handleEvaluation(tokens, index);
    _setVariableHelper(variableKey, variableValue);
}

bool isInteger(const std::string &str)
{
    try
    {
        size_t pos;
        std::stoi(str, &pos);
        return pos == str.size();
    }
    catch (...)
    {
        return false;
    }
}

Operand MacroSystem::_parseOperand(std::string token)
{
    if ((token[0] == '.') || (token[0] == '$'))
    {
        return MacroSystem::getVariable(token);
    }
    else if (isInteger(token))
    {
        int n = std::stoi(token);
        while (n < 0)
        {
            n = 256 - n;
        }
        return (unsigned int)n;
    }
    return token;
}

Operand MacroSystem::_handleEvaluation(std::vector<std::string> &tokens, size_t &index)
{
    return (unsigned int)0;
}
