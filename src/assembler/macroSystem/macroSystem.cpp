#include "macroSystem.hpp"
#include "../../utils/runtimeError.hpp"
#include "../../utils/syntaxError.hpp"
#include "../../utils/stringUtils.hpp"
#include "../../utils/typeError.hpp"
#include "compilationError.hpp"
#include <cassert>
#include <variant>

Operand getNodeValue(AST::Node *node)
{
    if (node->nodeType == AST::NodeType::INT)
    {
        return node->intValue;
    }
    else if (node->nodeType == AST::NodeType::STRING)
    {
        return node->identifier;
    }
    throw CompilationError(ErrorStage::Codegen,
                           node->begin,
                           node->end,
                           "Operand cannot be part of an expression");
}

MacroSystem::MacroSystem(AST::Node *node)
{
    _astNode = node;
    _callStack.push(AstLocation{
        node,
        0});
    _currentStack = -1; // pushStack automatically increments this
    pushVariableStack();
}

std::string MacroSystem::getLine()
{
    auto currNode = _getCurrNode();

    std::string result = _getLineHelper(currNode);
    _nextNode();
    return result;
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

void MacroSystem::pushVariableStack()
{
    VariableMap variables;
    _variables.push_back(variables);
    _currentStack++;
}

void MacroSystem::popVariableStack()
{
    if (_currentStack <= 0)
    {
        throw RuntimeError("Cannot pop a stack that doesn't exist");
    }
    _variables.pop_back();
    _currentStack--;
}

AST::Node *MacroSystem::getVariable(std::string symbol)
{
    return _getVariableHelper(symbol, _currentStack);
}

bool MacroSystem::done()
{
    return _callStack.size() == 0;
}

std::string MacroSystem::_getLineHelper(AST::Node *currNode)
{
    auto firstChild = currNode->children[0].get();

    if (firstChild->nodeType == AST::NodeType::ASSIGNMENT)
    {
        _evaluateAssignment(firstChild);
        _nextNode();
        return _getLineHelper(_getCurrNode());
    }

    auto result = _evaluateExpression(firstChild);

    if (std::holds_alternative<int>(result))
    {
        return std::to_string(std::get<int>(result));
    }
    else
    {
        return std::get<std::string>(result);
    }
}

Operand MacroSystem::_evaluateExpression(AST::Node *node)
{
    auto it = operations.find(node->nodeType);
    if (it == operations.end())
    {
        if (node->nodeType == AST::NodeType::IDENTIFIER)
        {
            AST::Node *variableValue = getVariable(node->identifier);
            return getNodeValue(variableValue);
        }
        else if (node->nodeType == AST::NodeType::BLOCK)
        {
            return _evaluateBlock(node);
        }
        return getNodeValue(node);
    }

    if (node->children.size() != 2)
    {
        throw CompilationError(ErrorStage::Codegen,
                               node->begin,
                               node->end,
                               "Expressions must have two operands");
    }

    auto lhs = _evaluateExpression(node->children[0].get());
    auto rhs = _evaluateExpression(node->children[1].get());

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

Operand MacroSystem::_evaluateBlock(AST::Node *node)
{
    assert(node->children.size() > 0);
    auto child = node->children[0].get();
    if (child->nodeType == AST::NodeType::LINE)
    {
        _pushCallStack(node);
        std::string resultLine = _getLineHelper(child);
        return resultLine;
    }
    else
    {
        return _evaluateExpression(child);
    }
}

void MacroSystem::_evaluateAssignment(AST::Node *node)
{
    assert(node->children.size() == 2);
    std::string identifier = node->children[0]->identifier;
    auto val = node->children[1].get();
    setVariable(identifier, val);
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

void MacroSystem::_pushCallStack(AST::Node *node)
{
    AstLocation loc = {
        node,
        0};
    _callStack.push(loc);
}

void MacroSystem::_popCallStack()
{
    _callStack.pop();
}

AST::Node *MacroSystem::_getCurrNode()
{
    auto loc = this->_callStack.top();
    assert(loc.node->children.size() > 0);
    auto child = loc.node->children[loc.index].get();
    return child;
}

void MacroSystem::_nextNode()
{
    if (done())
        return;

    auto &loc = this->_callStack.top();
    loc.index++;
    while (loc.index >= loc.node->children.size() && !done())
    {
        _popCallStack();
        loc.index++;
    }
}