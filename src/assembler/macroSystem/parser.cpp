#include "parser.hpp"

std::vector<std::unique_ptr<Parser::AST>> groupParentheses(std::vector<AsmMacroLexer::Token> tokens, size_t startingIndex = 0)
{
    std::vector<std::unique_ptr<Parser::AST>> parseResult = {};
    for (size_t i = startingIndex; i < tokens.size(); i++)
    {
        AsmMacroLexer::Token token = tokens[i];
        if (token.type == AsmMacroLexer::TokenType::OPENINGPARENTHESE)
        {
            Parser::AST tree = {
                token.begin,
                0,
                Parser::NodeType::BLOCK,
                groupParentheses(tokens, i),
                std::string(""),
                0,
            };
        }
    }
}

std::vector<std::unique_ptr<Parser::AST>> Parser::parseTokens(std::vector<AsmMacroLexer::Token> tokens)
{
    size_t blockBaseIndex = 0;
    std::vector<std::unique_ptr<Parser::AST>> tree = {};

    for (size_t i = 0; i < tokens.size() - 1; i++)
    {
        AsmMacroLexer::Token currToken = tokens[i];
        AsmMacroLexer::Token nextToken = tokens[i + 1];
    }
    return tree;
}