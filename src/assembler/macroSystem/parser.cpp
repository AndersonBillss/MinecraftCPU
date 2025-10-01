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
    AsmMacroLexer::Token token = tokens[currIndex + 1];
    return token.type == AsmMacroLexer::TokenType::OPERATOR && token.data == "=";
}
void handleFunction(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
    auto baseNode = std::make_unique<Parser::AST>(Parser::AST{
        tokens[currIndex].begin,
        {0, 0},
        Parser::NodeType::ASSIGNMENT,
        {},
        "",
        0});

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

    functionNode->children.push_back(std::move(parameterNode));
    functionNode->children.push_back(std::move(blockNode));
    baseNode->children.push_back(std::move(functionNode));
    tree.children.push_back(std::move(baseNode));

    currIndex += 1;
    while (currIndex < tokens.size())
    {

        currIndex++;
    }
}
void handleAssignment(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
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