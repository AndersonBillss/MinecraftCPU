#include "macroSystem.hpp"
#include "../../utils/runtimeError.hpp"
#include "../../utils/stringUtils.hpp"
#include "./lexer.hpp"

MacroSystem::MacroSystem()
{
    _currentStack = -1; // pushStack automatically increments this
    pushStack();
}

void MacroSystem::setNumber(std::string symbol, int value)
{
    _setVariableHelper(symbol, {type : number, value : value}, _currentStack);
}

void MacroSystem::setMacro(std::string symbol, std::string value)
{
    _setVariableHelper(symbol, {type : macro, value : value}, _currentStack);
}

void MacroSystem::setLabel(std::string symbol, int value)
{
    _setVariableHelper(symbol, {type : label, value : value});
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

Variable MacroSystem::getVariable(std::string symbol)
{
    return _getVariableHelper(symbol, _currentStack);
}

Variable MacroSystem::_getVariableHelper(std::string symbol, size_t stackIndex)
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

void MacroSystem::_setVariableHelper(std::string symbol, Variable value, size_t stackIndex)
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
            index++;
        }
    }
    return "";
}

void MacroSystem::_handleAssignment(std::vector<std::string> &tokens, int &index)
{
}

std::string MacroSystem::_handleEvaluation(std::vector<std::string> &tokens, int &index)
{
    return std::string();
}
