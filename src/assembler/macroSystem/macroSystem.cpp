#include "macroSystem.hpp"
#include "../../utils/runtimeError.hpp"

void MacroSystem::evaluate(std::string block)
{
}

int MacroSystem::getValue(std::string symbol, size_t stackIndex = 0)
{
    SymbolMap top = _values[stackIndex];

    auto it = top.find(symbol);
    if (it != top.end())
    {
        int value = it->second;
        return value;
    }
    else
    {
        if (stackIndex != 0)
            return getValue(symbol);
        throw RuntimeError("Variable not found: " + symbol);
    }
}

void MacroSystem::setValue(std::string symbol, int value, size_t stackIndex = 0)
{
    _values[stackIndex][symbol] = value;
}

std::string MacroSystem::getMacro(std::string symbol)
{
    auto it = _macros.find(symbol);
    if (it != _macros.end())
    {
        std::string value = it->second;
        return value;
    }
    else
    {
        throw RuntimeError("Macro not found: " + symbol);
    }
}

void MacroSystem::setMacro(std::string symbol, std::string value)
{
    _macros[symbol] = value;
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
