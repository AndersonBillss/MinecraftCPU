#include "macroSystem.hpp"
#include "../../utils/runtimeError.hpp"
#include "../../utils/syntaxError.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/typeError.hpp"
#include "compilationError.hpp"
#include "operations.hpp"
#include <variant>

Operand evaluateExpression(AST::Node *node)
{
    auto it = operations.find(node->nodeType);
    if (it == operations.end())
    {
        if (node->nodeType == AST::NodeType::INT)
        {
            return node->intValue;
        }
        else if (node->nodeType == AST::NodeType::STRING)
        {
            return node->identifier;
        }
        else
        {
            throw CompilationError(ErrorStage::Codegen,
                                   node->begin,
                                   node->end,
                                   "Operand cannot be part of an expression");
        }
    }

    if (node->children.size() != 2)
    {
        throw CompilationError(ErrorStage::Codegen,
                               node->begin,
                               node->end,
                               "Expressions must have two operands");
    }

    auto lhs = evaluateExpression(node->children[0].get());
    auto rhs = evaluateExpression(node->children[1].get());

    auto fn = it->second;
    try
    {
        auto result = fn(lhs, rhs);
        return result;
    }
    catch (TypeError &e)
    {
        throw CompilationError(ErrorStage::Codegen,
                               node->begin,
                               node->end,
                               e.what());
    }
}


MacroSystem::MacroSystem(AST::Node *node)
{
    _astNode = node;
    _astStack.push(AstLocation{
        node,
        0});
    _currentStack = -1; // pushStack automatically increments this
    pushStack();
}

std::string MacroSystem::getLine()
{
    auto currNode = _getCurrNode();

    auto result = evaluateExpression(currNode->children[0]->children[0].get());
    if(std::holds_alternative<int>(result)) {
        return std::to_string(std::get<int>(result));
    } else {
        return std::get<std::string>(result);
    }
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

bool MacroSystem::done()
{
    return _astStack.size() == 0;
}

void MacroSystem::setVariable(std::string symbol, AST::Node *value, size_t stackIndex)
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

AST::Node *MacroSystem::getVariable(std::string symbol)
{
    return _getVariableHelper(symbol, _currentStack);
}

AST::Node *MacroSystem::_getVariableHelper(std::string symbol, size_t stackIndex)
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

AST::Node *MacroSystem::_getCurrNode()
{
    auto loc = this->_astStack.top();
    auto child = loc.node->children[loc.index].get();
    return child;
}
