#include "oldMacroSystem.hpp"
#include "../../utils/runtimeError.hpp"
#include "../../utils/syntaxError.hpp"
#include "../../utils/stringUtils.hpp"
#include "oldLexer.hpp"
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

void MacroSystem::setString(std::string symbol, std::string value)
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

std::string getUntilNewLine(std::vector<std::string> &tokens, size_t &index)
{
    std::string result = "";
    size_t firstIndex = index;
    while (tokens[index] != "\n")
    {
        if (firstIndex != index)
        {
            result += " ";
        }
        result += tokens[index];
        index++;
    }
    return result;
}

bool isFunctionAssignment(const std::vector<std::string> &tokens, size_t index)
{
    for (size_t i = index; i < tokens.size(); i++)
    {
        std::string token = tokens[i];
        if (token == "=>")
            return true;
        if (token == "\n")
            return false;
    }
    return false;
}

void MacroSystem::_handleAssignment(std::vector<std::string> &tokens, size_t &index)
{
    std::string variableKey = tokens[index];
    index += 2; // Move the index past the equal sign
    Operand variableValue;
    if (isFunctionAssignment(tokens, index))
    {
        variableValue = getUntilNewLine(tokens, index);
    }
    else
    {
        variableValue = _handleEvaluation(tokens, index);
    }
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

Operand MacroSystem::_parseOperand(std::vector<std::string> &tokens, size_t &index)
{
    std::string token = tokens[index];
    if (token[0] == '$')
    {
        Operand result = MacroSystem::getVariable(token);
        if (std::holds_alternative<std::string>(result))
        {
            std::string &strResult = std::get<std::string>(result);
            if ((strResult[0] == '$') || (strResult[0] == '='))
            {
                return _evaluateFunction(strResult, tokens, index);
            }
        }
        return result;
    }
    else if (token[0] == '(')
    {
        size_t zero = 0;
        std::vector<std::string> parsed = {stringUtils::trim(_evaluateHelper(token, 1, token.size() - 1))};
        return _parseOperand(parsed, zero);
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

std::string MacroSystem::_evaluateHelper(const std::string &block, size_t startingIndex, size_t endingIndex)
{
    std::vector<std::string> tokens = OldAsmMacroLexer::tokenize(block, startingIndex, endingIndex);
    std::string evaluation = "";
    size_t index = 0;
    while (index < tokens.size())
    {
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
                std::string stringEval = std::get<std::string>(eval);
                evaluation += stringEval;
            }
            evaluation += "\n";
        }
    }
    return stringUtils::trim(evaluation);
}

Operand MacroSystem::_evaluateFunction(std::string function, std::vector<std::string> &tokens, size_t &index)
{
    pushStack();
    std::vector<std::string> split = stringUtils::split(function, "=>");
    if (split.size() > 2)
    {
        throw RuntimeError("Function has multiple '=>' symbols");
    }
    std::vector<std::string> args = stringUtils::split(split[0]);
    for (size_t i = 0; i < args.size(); i++)
    {
        index++;
        std::string arg = args[i];
        if ((arg[0] != '$') && (arg[0] != '.'))
        {
            throw SyntaxError("Variables must start with '$' or '.'");
        }
        if ((tokens.size() < index) || (tokens[index] == "\n"))
        {
            throw RuntimeError("Function has too few arguments");
        }
        Operand value = _parseOperand(tokens, index);
        _setVariableHelper(arg, value, _currentStack);
    }
    std::vector<std::string> evaluated = {evaluate(split[1])};
    size_t zero = 0;
    Operand result = _parseOperand(evaluated, zero);
    popStack();
    return result;
}

Operand MacroSystem::_handleEvaluation(std::vector<std::string> &tokens, size_t &index)
{
    if (tokens[index] == "\n")
    {
        return "";
    }
    Operand evaluation = _parseOperand(tokens, index);
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
            evaluation = op(evaluation, _parseOperand(tokens, index));
        }
        else
        {
            evaluation = addOperation(evaluation, " ");
            evaluation = addOperation(evaluation, _parseOperand(tokens, index));
        }
        index++;
    }
    return evaluation;
}

std::string MacroSystem::replaceLocationSymbols(const std::string &block)
{
    std::unordered_map<std::string, int> symbols;
    std::string result = "";
    std::vector<std::string> tokens = OldAsmMacroLexer::tokenize(block);
    size_t lineNum = 0;
    size_t sinceLineIncrease = 0;
    for (std::string &token : tokens)
    {
        if (token == "\n")
        {
            lineNum++;
            sinceLineIncrease = 0;
        }
        else if (sinceLineIncrease == 0 && token[0] == '.')
        {
            symbols[token] = lineNum;
        }
        else
        {
            sinceLineIncrease++;
        }
    }
    sinceLineIncrease = 0;
    for (std::string &token : tokens)
    {
        if (token == "\n")
        {
            lineNum++;
            sinceLineIncrease = 0;
            result += token;
        }
        else if (token[0] == '.')
        {
            if (sinceLineIncrease != 0)
            {
                auto it = symbols.find(token);
                if (it == symbols.end())
                {
                    throw RuntimeError("Symbol not found: " + token);
                }
                result += " " + std::to_string(it->second);
                sinceLineIncrease++;
            }
        }
        else
        {
            if (sinceLineIncrease != 0)
            {
                result += " ";
            }
            result += token;
            sinceLineIncrease++;
        }
    }
    return stringUtils::trim(result);
}

std::string MacroSystem::evaluate(const std::string &block)
{
    return _evaluateHelper(block, 0, block.size());
}
