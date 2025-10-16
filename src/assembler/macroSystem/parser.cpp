#include <unordered_map>
#include <unordered_set>
#include "parser.hpp"
#include "compilationError.hpp"

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
    if (_tokens.size() + 2 >= _currIndex - 1)
    {
        return false;
    }
    AsmMacroLexer::Token token = _tokens[_currIndex + 2];
    return token.type == AsmMacroLexer::TokenType::OPERATOR && token.data == "=";
}
void Parser::_handleFunction()
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

    _tree.children.push_back(std::move(functionNode));
    functionNode->children.push_back(std::move(functionNode));
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
}
void Parser::_handleAssignment()
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
        _handleFunction();
    }
    else
    {
        _handleExpression();
    }
}
void Parser::_handleParentheses()
{
    _currIndex++;
    if (_currIndex >= _tokens.size())
    {
        throw CompilationError(
            ErrorStage::Parsing,
            _tokens[_currIndex].begin,
            _tokens[_currIndex].end,
            "Unmatched parentheses");
    }
    auto blockNode = std::make_unique<Parser::AST>(Parser::AST{
        _tokens[_currIndex].begin,
        {0, 0},
        Parser::NodeType::BLOCK,
        {},
        "",
        0});
    _handleLine();
    blockNode->end = _tokens[_currIndex].end;
    _currIndex++;
    if (_currIndex >= _tokens.size())
    {
        throw CompilationError(
            ErrorStage::Parsing,
            _tokens[_currIndex].begin,
            _tokens[_currIndex].end,
            "Unmatched parentheses");
    }
}

size_t operatorPrecedence(Parser::NodeType opType)
{
    const std::vector<std::unordered_set<Parser::NodeType>> orderOfOperations = {
        {Parser::NodeType::MULTIPLY, Parser::NodeType::DIVIDE},
        {Parser::NodeType::ADD, Parser::NodeType::SUB},
        {Parser::NodeType::AND},
        {Parser::NodeType::XOR},
        {Parser::NodeType::OR},
    };
    for (size_t i = 0; i < orderOfOperations.size(); i++)
    {
        std::unordered_set<Parser::NodeType> set = orderOfOperations[i];
        auto it = set.find(opType);
        if (it != set.end())
            return i + 1; // Operation is found
    }
    return 0;
}

Parser::NodeType operatorType(AsmMacroLexer::Token token)
{
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

void Parser::_handleFirstOperand()
{
    AsmMacroLexer::Token token = _tokens[_currIndex];
    if (token.type == AsmMacroLexer::TokenType::OPENINGPARENTHESE)
    {
        _handleParentheses();
    }
    else
    {
        // TODO: Get the correct type and value for nodeType
        Parser::NodeType nodeType = Parser::NodeType::STRING;
        if (token.type == AsmMacroLexer::TokenType::SYMBOL)
        {
            nodeType = Parser::NodeType::IDENTIFIER;
        }
        auto firstOperand = std::make_unique<Parser::AST>(Parser::AST{
            _tokens[_currIndex].begin,
            _tokens[_currIndex].end,
            nodeType,
            {},
            "", // TODO: Get the value of this operand
            0});

        _tree.children.push_back(std::move(firstOperand));
    }
}

Parser::NodeType Parser::_handleOpType()
{
    auto token = _tokens[_currIndex];
    if (token.type == AsmMacroLexer::TokenType::OPERATOR)
    {
        _currIndex++;
        return operatorType(token);
    }
    return Parser::NodeType::CONCAT;
}

void Parser::_handleExpression()
{
    size_t lastOperatorPrecedence = 0;

    AsmMacroLexer::Token lastToken;
    while (_currIndex < _tokens.size())
    {
        std::unique_ptr<Parser::AST> opNode = std::make_unique<Parser::AST>(Parser::AST{
            _tokens[_currIndex].begin,
            {0, 0},
            Parser::NodeType::UNDEFINED,
            {},
            _tokens[0].data,
            0});
        _handleFirstOperand();
        opNode->nodeType = _handleOpType();
    }
}
void Parser::_handleLine()
{
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
        _handleFunction();
    }
    else if (_isAssignment())
    {
        _handleAssignment();
    }
    else
    {
        _handleExpression();
    }
}

Parser::AST Parser::parseTokens(std::vector<AsmMacroLexer::Token> &_tokens)
{
    size_t blockBaseIndex = 0;
    Parser::AST _tree = {
        {0, 0},
        {0, 0},
        Parser::NodeType::BLOCK,
        {},
        "",
        0};
    for (size_t i = 0; i < _tokens.size(); i++)
    {
        _handleLine();
    }

    return _tree;
}
