#include "catch_amalgamated.hpp"
#include "../../src/assembler/macroSystem/lexer.hpp"
#include "../../src/assembler/macroSystem/parser.hpp"
#include <string>

std::string stringifyTree(const Parser::AST &tree, std::string tabs)
{
    std::unordered_map<Parser::NodeType, std::string> nodeToString{
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
    std::string stringifiedNodeType = "?????";
    auto it = nodeToString.find(tree.nodeType);
    if (it != nodeToString.end())
    {
        stringifiedNodeType = it->second;
    }
    std::string stringifiedNumber = "";
    if (tree.nodeType == Parser::INT)
    {
        stringifiedNumber = std::to_string(tree.intValue) + " ";
    }
    std::string stringified = tabs + stringifiedNodeType + ": " + stringifiedNumber + tree.identifier + "\n";
    for (const auto &branch : tree.children)
    {
        stringified += stringifyTree(*branch, tabs + "  ");
    }
    return stringified;
}

inline std::ostream &operator<<(std::ostream &os,
                                const Parser::AST &tree)
{
    return os << stringifyTree(tree, "");
}

std::unique_ptr<Parser::AST> parseTokensHelper(std::string program)
{
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
    std::cout << *tree << std::endl;

    REQUIRE(true == true);
}