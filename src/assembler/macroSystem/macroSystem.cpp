#include "macroSystem.hpp"
#include "../../utils/runtimeError.hpp"
#include "../../utils/syntaxError.hpp"
#include "../../utils/stringUtils.hpp"
#include "lexer.hpp"
#include "operations.hpp"

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
    unsigned int val = std::get<unsigned int>(_getVariableHelper(symbol, _currentStack));
    return val;
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

void MacroSystem::_handleAssignment(std::vector<std::string> &tokens, size_t &index)
{
    std::string variableKey = tokens[index];
    index += 2; // Move the index past the equal sign
    Operand variableValue = _handleEvaluation(tokens, index);
    _setVariableHelper(variableKey, variableValue, _currentStack);
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
    else if (token[0] == '(')
    {
        return _parseOperand(evaluate(token, 1, token.size() - 1));
    }
    else if (isInteger(token))
    {
        int n = std::stoi(token);
        while (n < 0)
        {
            n += 256;
        }
        return (unsigned int)n;
    }
    return token;
}

Operand MacroSystem::_handleEvaluation(std::vector<std::string> &tokens, size_t &index)
{
    if (tokens[index] == "\n")
    {
        return "";
    }
    Operand evaluation = _parseOperand(tokens[index]);
    index++;
    while (index < tokens.size() && tokens[index] != "\n")
    {
        auto it = operations.find(tokens[index]);
        if (it != operations.end())
        {
            index++;
            if ((tokens[index] == "\n") || (tokens.size() < index))
            {
                throw SyntaxError("Line cannot end in operation");
            }
            auto op = it->second;
            evaluation = op(evaluation, _parseOperand(tokens[index]));
        }
        else
        {
            evaluation = addOperation(evaluation, " ");
            evaluation = addOperation(evaluation, _parseOperand(tokens[index]));
        }
        index++;
    }
    return evaluation;
}

std::string MacroSystem::evaluate(const std::string &block, size_t startingIndex, size_t endingIndex)
{
    std::vector<std::string> tokens = AsmMacroLexer::tokenize(block);
    std::string evaluation = "";
    size_t index = startingIndex;
    size_t end;
    if (endingIndex > startingIndex)
    {
    }
    else
    {
        end = tokens.size();
    }
    while (index < end)
    {
        if (tokens[index][0] == '.')
        {
            setLabel(tokens[index], _currLineNum);
            index++;
        }
        if ((index + 1 < tokens.size()) && (tokens[index + 1] == "="))
        {
            _handleAssignment(tokens, index);
        }
        else if (tokens[index] == "\n")
        {
            index++;
        }
        else
        {
            Operand eval = _handleEvaluation(tokens, index);
            if (std::holds_alternative<unsigned int>(eval))
            {
                evaluation += std::to_string(std::get<unsigned int>(eval));
            }
            else
            {
                evaluation += std::get<std::string>(eval);
            }
            evaluation += "\n";
        }
    }
    return evaluation;
}
