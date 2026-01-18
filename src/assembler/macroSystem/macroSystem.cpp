#include "macroSystem.hpp"
#include "../../utils/runtimeError.hpp"
#include "../../utils/syntaxError.hpp"
#include "../../utils/stringUtils.hpp"
#include "operations.hpp"

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

    if (currNode->nodeType == AST::NodeType::ASSIGNMENT)
    {
        
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
