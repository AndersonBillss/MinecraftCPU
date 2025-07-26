#include "macroSystem.hpp"
#include "../../utils/runtimeError.hpp"

MacroSystem::MacroSystem()
{
    _currentStack = -1; // pushStack automatically increments this
    pushStack();   
}

void MacroSystem::evaluate(std::string block)
{
}

int MacroSystem::getValue(std::string symbol)
{
    return _getValueHelper(symbol, _currentStack);
}

void MacroSystem::setValue(std::string symbol, int value)
{
    SymbolMap &selectedScope = _values[0];

    auto it = selectedScope.find(symbol);
    if (it != selectedScope.end())
    {
        selectedScope[symbol] = value;
    }
    else
    {
        _values[_currentStack][symbol] = value;
    }
}

std::string MacroSystem::getMacro(std::string symbol)
{
    return _getMacroHelper(symbol, _currentStack);
}

void MacroSystem::setMacro(std::string symbol, std::string value)
{
    VariableMap &selectedScope = _macros[0];

    auto it = selectedScope.find(symbol);
    if (it != selectedScope.end())
    {
        selectedScope[symbol] = value;
    }
    else
    {
        _macros[_currentStack][symbol] = value;
    }
}

int MacroSystem::getLabel(std::string symbol)
{
    auto it = _labels.find(symbol);
    if (it != _labels.end())
    {
        int value = it->second;
        return value;
    }
    else
    {
        throw RuntimeError("Label not found: " + symbol);
    }
}

void MacroSystem::setLabel(std::string symbol, int value)
{
    _labels[symbol] = value;
}

void MacroSystem::pushStack()
{
    SymbolMap symbols;
    _values.push_back(symbols);
    VariableMap variables;
    _macros.push_back(variables);
    _currentStack++;
}

void MacroSystem::popStack()
{
    if(_currentStack <= 0){
        throw RuntimeError("Cannot pop a stack that doesn't exist");
    }
    _values.pop_back();
    _macros.pop_back();
    _currentStack--;
}

int MacroSystem::_getValueHelper(std::string symbol, size_t stackIndex)
{
    SymbolMap &scope = _values[stackIndex];

    auto it = scope.find(symbol);
    if (it != scope.end())
    {
        int value = it->second;
        return value;
    }
    else
    {
        if (stackIndex != 0)
            return _getValueHelper(symbol);
        throw RuntimeError("Variable not found: " + symbol);
    }
}

std::string MacroSystem::_getMacroHelper(std::string symbol, size_t stackIndex)
{
    VariableMap &scope = _macros[stackIndex];

    auto it = scope.find(symbol);
    if (it != scope.end())
    {
        std::string value = it->second;
        return value;
    }
    else
    {
        if (stackIndex != 0)
            return _getMacroHelper(symbol);
        throw RuntimeError("Variable not found: " + symbol);
    }
}
