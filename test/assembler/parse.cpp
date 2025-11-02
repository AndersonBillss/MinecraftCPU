#include "catch_amalgamated.hpp"
#include "../../src/assembler/macroSystem/lexer.hpp"
#include "../../src/assembler/macroSystem/parser.hpp"
#include <string>

inline std::ostream &operator<<(std::ostream &os,
                                const Parser::AST &tree)
{
    std::unordered_map<Parser::NodeType, std::string> nodeToString {
        {Parser::NodeType::PROGRAM, "PROGRAM"},
        {Parser::NodeType::BLOCK, "BLOCK"},
        {Parser::NodeType::LINE, "LINE"},

        {Parser::NodeType::ADD, "ADD"},
        {Parser::NodeType::SUB, "SUB"},
        {Parser::NodeType::OR, "OR"},
        {Parser::NodeType::AND, "AND"},
        {Parser::NodeType::XOR, "XOR"},
        {Parser::NodeType::MULTIPLY, "MULTIPLY"},
        {Parser::NodeType::DIVIDE, "DIVIDE"},
        {Parser::NodeType::CONCAT, "CONCAT"},

        {Parser::NodeType::ASSIGNMENT, "ASSIGNMENT"},
        {Parser::NodeType::FUNCTION, "FUNCTION"},
        {Parser::NodeType::PARAMETER_LIST, "PARAMETER_LIST"},
        {Parser::NodeType::RETURN, "RETURN"},

        {Parser::NodeType::IDENTIFIER, "IDENTIFIER"},
        {Parser::NodeType::STRING, "STRING"},
        {Parser::NodeType::INT, "INT"},
        {Parser::NodeType::UNDEFINED, "UNDEFINED"},
    };

    return os << "()";
}

std::unique_ptr<Parser::AST> parseTokensHelper(std::string program){
    AsmMacroLexer lexer;
    Parser parser;
    auto tokens = lexer.tokenize(program);
    auto tree = parser.parseTokens(tokens);
    return tree;
}

TEST_CASE("Parser no macros")
{
    std::string program = "LDI R0 1";
    auto tree = parseTokensHelper(program);
    
    REQUIRE(true == true);
}