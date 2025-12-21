#include <unordered_map>
#include <unordered_set>
#include "parser.hpp"
#include "compilationError.hpp"
#include "../../utils/parseUtils.hpp"

bool Parser::_isFunction()
{
    for (size_t i = _currIndex; i < _tokens.size(); i++)
    {
        if (_tokens[i].type == AsmMacroLexer::TokenType::ENDLINE)
        {
            return false;
        }
        else if (_tokens[i].type == AsmMacroLexer::TokenType::OPERATOR && _tokens[i].data == "=>")
        {
            return true;
        }
    }
    return false;
}
bool Parser::_isAssignment()
{
    if (_tokens.size() + 3 >= _currIndex)
    {
        return false;
    }
    AsmMacroLexer::Token token = _tokens[_currIndex + 2];
    return token.type == AsmMacroLexer::TokenType::OPERATOR && token.data == "=";
}
std::unique_ptr<Parser::AST> Parser::_handleFunction()
{
    //   Function AST structure
    //
    //          function
    //         _____|_____
    //        |           |
    //   parameters     block

    auto functionNode = std::make_unique<Parser::AST>(Parser::AST{
        _tokens[_currIndex].begin,
        {0, 0},
        Parser::NodeType::FUNCTION,
        {},
        "",
        0});
    // _functions.emplace(functionNode->identifier, functionNode); // TODO: Implement function calls

    auto parameterNode = std::make_unique<Parser::AST>(Parser::AST{
        _tokens[_currIndex + 2].begin,
        {0, 0},
        Parser::NodeType::PARAMETER_LIST,
        {},
        "",
        0});

    auto blockNode = std::make_unique<Parser::AST>(Parser::AST{
        _tokens[_currIndex].begin,
        {0, 0},
        Parser::NodeType::BLOCK,
        {},
        "",
        0});

    functionNode->children.push_back(std::move(parameterNode));
    functionNode->children.push_back(std::move(blockNode));

    bool isParameter = true;
    while (_currIndex < _tokens.size())
    {
        if (_tokens[_currIndex].type == AsmMacroLexer::TokenType::ENDLINE)
        {
            throw CompilationError(ErrorStage::Parsing,
                                   _tokens[_currIndex].begin,
                                   _tokens[_currIndex].end,
                                   "Function terminated too soon");
        }
        if (isParameter)
        {
            parameterNode->children.push_back(
                std::make_unique<Parser::AST>(Parser::AST{
                    _tokens[_currIndex].begin,
                    {0, 0},
                    Parser::NodeType::IDENTIFIER,
                    {},
                    _tokens[0].data,
                    0}));
        }
        else if (_tokens[_currIndex].type == AsmMacroLexer::TokenType::OPERATOR &&
                 _tokens[_currIndex].data == "=>")
        {
            _currIndex++;
            if (_tokens.size() >= _currIndex || _tokens[_currIndex + 1].type == AsmMacroLexer::TokenType::ENDLINE)
            {
                throw CompilationError(ErrorStage::Parsing,
                                       _tokens[_currIndex].begin,
                                       _tokens[_currIndex].end,
                                       "Function has no output");
            }
            _handleLine();
        }

        _currIndex++;
    }
    return functionNode;
}
std::unique_ptr<Parser::AST> Parser::_handleAssignment()
{
    // Assignment AST structure
    //
    //       assignment
    //      _____|_____
    //     |           |
    //   label       block
    //
    //  ---------OR----------
    //
    //       assignment
    //      _____|_____
    //     |           |
    //   label       function

    if (_tokens.size() - 1 <= _currIndex + 1 || _tokens[_currIndex + 1].type == AsmMacroLexer::TokenType::ENDLINE)
    {
        throw CompilationError(ErrorStage::Parsing,
                               _tokens[_tokens.size() - 1].begin,
                               _tokens[_tokens.size() - 1].end,
                               "No equal sign for assignment");
    }
    if (_tokens.size() - 1 <= _currIndex + 2)
    {
        throw CompilationError(ErrorStage::Parsing,
                               _tokens[_tokens.size() - 1].begin,
                               _tokens[_tokens.size() - 1].end,
                               "Label not assigned to value");
    }

    auto assignmentNode = std::make_unique<Parser::AST>(Parser::AST{
        _tokens[_currIndex].begin,
        {0, 0},
        Parser::NodeType::ASSIGNMENT,
        {},
        "",
        0});

    auto labelNode = std::make_unique<Parser::AST>(Parser::AST{
        _tokens[_currIndex].begin,
        {0, 0},
        Parser::NodeType::IDENTIFIER,
        {},
        _tokens[0].data,
        0});

    _currIndex += 2;
    auto valueNode = std::make_unique<Parser::AST>(Parser::AST{
        _tokens[_currIndex].begin,
        {0, 0},
        Parser::NodeType::BLOCK,
        {},
        "",
        0});

    if (_isFunction())
    {
        valueNode->children.push_back(_handleFunction());
    }
    else
    {
        valueNode->children.push_back(_handleExpression());
    }
    return assignmentNode;
}
std::unique_ptr<Parser::AST> Parser::_handleParentheses()
{
    auto blockNode = std::make_unique<Parser::AST>(Parser::AST{
        _tokens[_currIndex].begin,
        {0, 0},
        Parser::NodeType::BLOCK,
        {},
        "",
        0});

    _currIndex++;
    while (_tokens[_currIndex].type != AsmMacroLexer::TokenType::CLOSINGPARENTHESE)
    {
        if (_currIndex >= _tokens.size())
        {
            throw CompilationError(
                ErrorStage::Parsing,
                _tokens[_currIndex].begin,
                _tokens[_currIndex].end,
                "Unmatched parentheses");
        }
        if (_tokens[_currIndex].type == AsmMacroLexer::TokenType::ENDLINE)
        {
            blockNode->children.push_back(_handleLine());
        }
        else
        {
            blockNode->children.push_back(_handleExpression());
        }
    }
    blockNode->end = _tokens[_currIndex].end;
    _currIndex++;
    return blockNode;
}

int operatorPrecedence(Parser::NodeType opType)
{
    const std::vector<std::unordered_set<Parser::NodeType>> orderOfOperations = {
        {Parser::NodeType::MULTIPLY, Parser::NodeType::DIVIDE},
        {Parser::NodeType::ADD, Parser::NodeType::SUB},
        {Parser::NodeType::AND},
        {Parser::NodeType::XOR},
        {Parser::NodeType::OR},
        {Parser::NodeType::CONCAT},
    };
    for (size_t i = 0; i < orderOfOperations.size(); i++)
    {
        std::unordered_set<Parser::NodeType> set = orderOfOperations[i];
        auto it = set.find(opType);
        if (it != set.end())
            return orderOfOperations.size() - i; // Operation is found
    }
    return 0;
}

Parser::NodeType operatorType(AsmMacroLexer::Token token)
{
    if (token.type != AsmMacroLexer::TokenType::OPERATOR)
    {
        return Parser::NodeType::CONCAT;
    }
    const std::unordered_map<std::string, Parser::NodeType> stringToOperator = {
        {"*", Parser::NodeType::MULTIPLY},
        {"/", Parser::NodeType::DIVIDE},
        {"+", Parser::NodeType::ADD},
        {"-", Parser::NodeType::SUB},
        {"&", Parser::NodeType::AND},
        {"^", Parser::NodeType::XOR},
        {"|", Parser::NodeType::OR},
    };

    auto it = stringToOperator.find(token.data);
    if (it == stringToOperator.end())
    {
        throw CompilationError(
            ErrorStage::Parsing,
            token.begin,
            token.end,
            "Invalid Operator: " + token.data);
    }
    return it->second;
}

int parseNumeric(AsmMacroLexer::Token token)
{
    try
    {
        return ParseUtils::parseInt(token.data, 8, false);
    }
    catch (ParseUtils::ParseError &e)
    {
        throw CompilationError(
            ErrorStage::Parsing,
            token.begin,
            token.end,
            std::string(e.what()));
    }
}

/// @brief Returns the next operand token and advances the current index
std::unique_ptr<Parser::AST> Parser::_parseAtom()
{
    AsmMacroLexer::Token token = _tokens[_currIndex];
    if (token.type == AsmMacroLexer::TokenType::OPENINGPARENTHESE)
    {
        return _handleParentheses();
    }

    Parser::NodeType nodeType;
    int intValue = 0;
    std::string stringValue = "";
    if (token.type == AsmMacroLexer::TokenType::SYMBOL)
    {
        nodeType = Parser::NodeType::IDENTIFIER;
    }
    else
    {
        nodeType = ParseUtils::isNumeric(token.data) ? Parser::NodeType::INT : Parser::NodeType::STRING;
        if (nodeType == Parser::NodeType::INT)
        {
            intValue = parseNumeric(token);
        }
        else
        {
            stringValue = token.data;
        }
    }

    auto operandNode = std::make_unique<Parser::AST>(Parser::AST{
        token.begin,
        token.end,
        nodeType,
        {},
        stringValue,
        intValue});

    _currIndex++;
    return operandNode;
}

Parser::NodeType Parser::_handleOpType()
{
    if (_currIndex >= _tokens.size())
    {
        return Parser::NodeType::UNDEFINED;
    }
    auto token = _tokens[_currIndex];
    if (token.type == AsmMacroLexer::TokenType::OPERATOR)
    {
        _currIndex++;
        return operatorType(token);
    }
    else if (
        token.type == AsmMacroLexer::TokenType::SYMBOL ||
        token.type == AsmMacroLexer::TokenType::VALUE ||
        token.type == AsmMacroLexer::TokenType::OPENINGPARENTHESE ||
        token.type == AsmMacroLexer::TokenType::LOCATIONMARKER)
    {
        return Parser::NodeType::CONCAT;
    }
    return Parser::NodeType::UNDEFINED;
}

bool Parser::_isExpressionEndingNonterminal()
{
    if (_currIndex >= _tokens.size())
        return true;

    auto type = _tokens[_currIndex].type;
    return type == AsmMacroLexer::TokenType::ENDLINE ||
           type == AsmMacroLexer::TokenType::CLOSINGPARENTHESE;
}

std::unique_ptr<Parser::AST> Parser::_handleExpressionHelper(std::unique_ptr<Parser::AST> lhs, int minPrecedence)
{
    if (_isExpressionEndingNonterminal())
    {
        return lhs;
    }
    auto lookahead = _tokens[_currIndex];
    while (!_isExpressionEndingNonterminal() &&
           operatorPrecedence(operatorType(lookahead)) >= minPrecedence)
    {
        auto op = lookahead;
        if (op.type == AsmMacroLexer::TokenType::OPERATOR)
            _currIndex++;

        if (_isExpressionEndingNonterminal())
        {
            return lhs;
        }
        auto rhs = _parseAtom();
        if (!_isExpressionEndingNonterminal())
        {
            lookahead = _tokens[_currIndex];
        }
        while (!_isExpressionEndingNonterminal())
        {
            auto opPrecedence = operatorPrecedence(operatorType(op));
            auto lookaheadPrecedence = operatorPrecedence(operatorType(lookahead));
            if (lookaheadPrecedence <= opPrecedence)
                break;

            rhs = _handleExpressionHelper(std::move(rhs), opPrecedence + (lookaheadPrecedence > opPrecedence ? 1 : 0));
            if (!_isExpressionEndingNonterminal())
            {
                lookahead = _tokens[_currIndex];
            }
            else
            {
                break;
            }
        }
        std::vector<std::unique_ptr<Parser::AST>> operands = {};
        operands.push_back(std::move(lhs));
        operands.push_back(std::move(rhs));
        lhs = std::make_unique<Parser::AST>(Parser::AST{
            lookahead.begin,
            {0, 0},
            operatorType(op),
            std::move(operands),
            "",
            0});
    }
    return lhs;
}
std::unique_ptr<Parser::AST> Parser::_handleExpression()
{
    return _handleExpressionHelper(_parseAtom(), 0);
}

std::unique_ptr<Parser::AST> Parser::_handleLine()
{
    std::unique_ptr<Parser::AST> lineNode = std::make_unique<Parser::AST>(Parser::AST{
        _tokens[_currIndex].begin,
        {0, 0},
        Parser::NodeType::LINE,
        {},
        "",
        0});

    std::unique_ptr<Parser::AST> node;
    if (_isFunction())
    {
        if (!_isAssignment())
        {
            throw CompilationError(
                ErrorStage::Parsing,
                _tokens[_currIndex].begin,
                _tokens[_currIndex].end,
                "A function must be assigned to a variable");
        }
        node = _handleFunction();
        lineNode->end = node->end;
    }
    else if (_isAssignment())
    {
        node = _handleAssignment();
    }
    else
    {
        node = _handleExpression();
    }
    lineNode->end = node->end;
    lineNode->children.push_back(std::move(node));
    return lineNode;
}

std::unique_ptr<Parser::AST> Parser::parseTokens(std::vector<AsmMacroLexer::Token> &tokens)
{
    this->_tokens = tokens;
    _root = std::make_unique<Parser::AST>(
        Parser::AST{tokens[_currIndex].begin,
                    {0, 0},
                    Parser::NodeType::PROGRAM,
                    {},
                    "",
                    0});

    _currIndex = 0;
    while (_currIndex < _tokens.size())
    {
        auto node = _handleLine();
        _root->children.push_back(std::move(node));
        _currIndex++;
    }
    _root->end = _root->children[_root->children.size() - 1]->end;

    return std::move(_root);
}
