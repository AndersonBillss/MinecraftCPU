#include "macroSystem.hpp"
#include "../../utils/runtimeError.hpp"
#include "../../utils/stringUtils.hpp"

MacroSystem::MacroSystem()
{
    _currentStack = -1; // pushStack automatically increments this
    pushStack();
}

std::string MacroSystem::evaluate(const std::string &block)
{
    if (block[0] == '$')
    {
        std::vector<std::string> splitBlock = stringUtils::split(block, "=");
    }
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
