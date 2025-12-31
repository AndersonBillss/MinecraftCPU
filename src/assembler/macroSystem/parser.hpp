#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include <iostream>
#include "AST.hpp"
#include "lexer.hpp"

class Parser
{
public:
    std::unique_ptr<AST::Node> parseTokens(std::vector<Lexer::Token> &tokens);

private:
    std::unique_ptr<AST::Node> _root;
    size_t _currIndex;
    std::vector<Lexer::Token> _tokens;
    std::unordered_map<std::string, AST::Node *> _functions;

    bool _isFunction();
    bool _isAssignment();
    bool _functionExists(std::string functionName);
    AST::NodeType _handleOpType();
    std::unique_ptr<AST::Node> _handleFunction(std::string functionName);
    bool _isFunctionCallEndingNonterminal();
    std::unique_ptr<AST::Node> _handleFunctionCall();
    std::unique_ptr<AST::Node> _handleAssignment();
    std::unique_ptr<AST::Node> _handleParentheses();
    std::unique_ptr<AST::Node> _parseAtom();
    bool _isExpressionEndingNonterminal();
    std::unique_ptr<AST::Node> _handleExpressionHelper(std::unique_ptr<AST::Node> lhs, int minPrecedence);
    std::unique_ptr<AST::Node> _handleExpression();
    std::unique_ptr<AST::Node> _handleLine();

public:
    Parser()
    {
        _currIndex = 0;
        _tokens = {};
    }
};
