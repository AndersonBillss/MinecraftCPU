#include <unordered_map>
#include <unordered_set>
#include "parser.hpp"
#include "compilationError.hpp"

bool isFunction(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t currIndex)
{
    for (size_t i = currIndex; i < tokens.size(); i++)
    {
        if (tokens[i].type == AsmMacroLexer::TokenType::ENDLINE)
        {
            return false;
        }
        else if (tokens[i].type == AsmMacroLexer::TokenType::OPERATOR && tokens[i].data == "=>")
        {
            return true;
        }
    }
    return false;
}
bool isAssignment(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t currIndex)
{
    if (tokens.size() + 2 >= currIndex - 1)
    {
        return false;
    }
    AsmMacroLexer::Token token = tokens[currIndex + 2];
    return token.type == AsmMacroLexer::TokenType::OPERATOR && token.data == "=";
}
void handleFunction(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
    //   Function AST structure
    //
    //          function
    //         _____|_____
    //        |           |
    //   parameters     block

    auto functionNode = std::make_unique<Parser::AST>(Parser::AST{
        tokens[currIndex].begin,
        {0, 0},
        Parser::NodeType::FUNCTION,
        {},
        "",
        0});

    auto parameterNode = std::make_unique<Parser::AST>(Parser::AST{
        tokens[currIndex + 2].begin,
        {0, 0},
        Parser::NodeType::PARAMETER_LIST,
        {},
        "",
        0});

    auto blockNode = std::make_unique<Parser::AST>(Parser::AST{
        tokens[currIndex].begin,
        {0, 0},
        Parser::NodeType::BLOCK,
        {},
        "",
        0});

    tree.children.push_back(std::move(functionNode));
    functionNode->children.push_back(std::move(functionNode));
    functionNode->children.push_back(std::move(parameterNode));
    functionNode->children.push_back(std::move(blockNode));

    bool isParameter = true;
    while (currIndex < tokens.size())
    {
        if (tokens[currIndex].type == AsmMacroLexer::TokenType::ENDLINE)
        {
            throw CompilationError(ErrorStage::Parsing,
                                   tokens[currIndex].begin,
                                   tokens[currIndex].end,
                                   "Function terminated too soon");
        }
        if (isParameter)
        {
            parameterNode->children.push_back(
                std::make_unique<Parser::AST>(Parser::AST{
                    tokens[currIndex].begin,
                    {0, 0},
                    Parser::NodeType::IDENTIFIER,
                    {},
                    tokens[0].data,
                    0}));
        }
        else if (tokens[currIndex].type == AsmMacroLexer::TokenType::OPERATOR &&
                 tokens[currIndex].data == "=>")
        {
            currIndex++;
            if (tokens.size() >= currIndex || tokens[currIndex + 1].type == AsmMacroLexer::TokenType::ENDLINE)
            {
                throw CompilationError(ErrorStage::Parsing,
                                       tokens[currIndex].begin,
                                       tokens[currIndex].end,
                                       "Function has no output");
            }
            handleLine(*blockNode, tokens, currIndex);
        }

        currIndex++;
    }
}
void handleAssignment(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
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

    if (tokens.size() - 1 <= currIndex + 1 || tokens[currIndex + 1].type == AsmMacroLexer::TokenType::ENDLINE)
    {
        throw CompilationError(ErrorStage::Parsing,
                               tokens[tokens.size() - 1].begin,
                               tokens[tokens.size() - 1].end,
                               "No equal sign for assignment");
    }
    if (tokens.size() - 1 <= currIndex + 2)
    {
        throw CompilationError(ErrorStage::Parsing,
                               tokens[tokens.size() - 1].begin,
                               tokens[tokens.size() - 1].end,
                               "Label not assigned to value");
    }

    auto assignmentNode = std::make_unique<Parser::AST>(Parser::AST{
        tokens[currIndex].begin,
        {0, 0},
        Parser::NodeType::ASSIGNMENT,
        {},
        "",
        0});

    auto labelNode = std::make_unique<Parser::AST>(Parser::AST{
        tokens[currIndex].begin,
        {0, 0},
        Parser::NodeType::IDENTIFIER,
        {},
        tokens[0].data,
        0});

    currIndex += 2;
    auto valueNode = std::make_unique<Parser::AST>(Parser::AST{
        tokens[currIndex].begin,
        {0, 0},
        Parser::NodeType::BLOCK,
        {},
        "",
        0});

    if (isFunction(tree, tokens, currIndex))
    {
        handleFunction(*valueNode, tokens, currIndex);
    }
    else
    {
        handleExpression(*valueNode, tokens, currIndex);
    }
}
void handleParentheses(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
    currIndex++;
    if (currIndex >= tokens.size())
    {
        throw CompilationError(
            ErrorStage::Parsing,
            tokens[currIndex].begin,
            tokens[currIndex].end,
            "Unmatched parentheses");
    }
    auto blockNode = std::make_unique<Parser::AST>(Parser::AST{
        tokens[currIndex].begin,
        {0, 0},
        Parser::NodeType::BLOCK,
        {},
        "",
        0});
    handleLine(tree, tokens, currIndex);
    blockNode->end = tokens[currIndex].end;
    currIndex++;
    if (currIndex >= tokens.size())
    {
        throw CompilationError(
            ErrorStage::Parsing,
            tokens[currIndex].begin,
            tokens[currIndex].end,
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

void handleFirstOperand(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
    AsmMacroLexer::Token token = tokens[currIndex];
    if (token.type == AsmMacroLexer::TokenType::OPENINGPARENTHESE)
    {
        handleParentheses(tree, tokens, currIndex);
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
            tokens[currIndex].begin,
            tokens[currIndex].end,
            nodeType,
            {},
            "", // TODO: Get the value of this operand
            0});

        tree.children.push_back(firstOperand);
    }
}

Parser::NodeType handleOpType(std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
    auto token = tokens[currIndex];
    if(token.type == AsmMacroLexer::TokenType::OPERATOR){
        currIndex++;
        return operatorType(token);
    }
    return Parser::NodeType::CONCAT;
}

void handleExpression(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
    std::unique_ptr<Parser::AST> rootNode = std::make_unique<Parser::AST>(tree);

    size_t lastOperatorPrecedence = 0;

    AsmMacroLexer::Token lastToken;
    while (currIndex < tokens.size())
    {
        std::unique_ptr<Parser::AST> opNode = std::make_unique<Parser::AST>(Parser::AST{
            tokens[currIndex].begin,
            {0, 0},
            Parser::NodeType::UNDEFINED,
            {},
            tokens[0].data,
            0});
        handleFirstOperand(*opNode, tokens, currIndex);
        opNode->nodeType = handleOpType(tokens, currIndex);
    }
}
void handleLine(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
    if (isFunction(tree, tokens, currIndex))
    {
        if (!isAssignment(tree, tokens, currIndex))
        {
            throw CompilationError(
                ErrorStage::Parsing,
                tokens[currIndex].begin,
                tokens[currIndex].end,
                "A function must be assigned to a variable");
        }
        handleFunction(tree, tokens, currIndex);
    }
    else if (isAssignment(tree, tokens, currIndex))
    {
        handleAssignment(tree, tokens, currIndex);
    }
    else
    {
        handleExpression(tree, tokens, currIndex);
    }
}

Parser::AST Parser::parseTokens(std::vector<AsmMacroLexer::Token> &tokens)
{
    size_t blockBaseIndex = 0;
    Parser::AST tree = {
        {0, 0},
        {0, 0},
        Parser::NodeType::BLOCK,
        {},
        "",
        0};
    for (size_t i = 0; i < tokens.size(); i++)
    {
        handleLine(tree, tokens, i);
    }

    return tree;
}