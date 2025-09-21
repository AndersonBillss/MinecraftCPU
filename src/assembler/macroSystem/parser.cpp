#include "parser.hpp"


std::vector<std::unique_ptr<Parser::AST>> Parser::parseTokens(std::vector<AsmMacroLexer::Token> tokens)
{
    std::vector<std::unique_ptr<AST>> tree;

    for (size_t i = 0; i < tokens.size() - 1; i++)
    {
        AsmMacroLexer::Token currToken = tokens[i];
        AsmMacroLexer::Token nextToken = tokens[i + 1];
    }
    return tree;
}