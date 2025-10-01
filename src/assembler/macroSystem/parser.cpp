#include "parser.hpp"

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
void handleExpression(Parser::AST &tree, std::vector<AsmMacroLexer::Token> &tokens, size_t &currIndex)
{
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
        handleExpression(tree, tokens, i);
    }

    return tree;
}