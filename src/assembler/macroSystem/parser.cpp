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
}
void handleExpression(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
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
        handleLine(tree, tokens, currIndex);
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