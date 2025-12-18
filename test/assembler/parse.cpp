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
    std::string stringified = tabs + stringifiedNodeType + " " +
                              tree.begin.stringify() + " " + tree.end.stringify() + ": " +
                              stringifiedNumber + tree.identifier + "\n";
    for (const auto &branch : tree.children)
    {
        stringified += stringifyTree(*branch, tabs + "  ");
    }
    return stringified;
}

inline std::ostream &operator<<(std::ostream &os,
                                const std::unique_ptr<Parser::AST> &tree)
{
    return os << stringifyTree(*tree, "");
}

inline bool operator==(const std::unique_ptr<Parser::AST> &a, const std::unique_ptr<Parser::AST> &b)
{
    if (
        !(
            a->begin == b->begin &&
            a->end == b->end &&
            a->identifier == b->identifier &&
            a->intValue == b->intValue &&
            a->nodeType == b->nodeType))
        return false;

    if (a->children.size() != b->children.size())
        return false;

    for (size_t i = 0; i < a->children.size(); i++)
    {
        auto &childA = a->children[i];
        auto &childB = b->children[i];
        bool childrenMatch = childA == childB;
        if (!childrenMatch)
            return false;
    }
    return true;
}

std::unique_ptr<Parser::AST> parseTokensHelper(std::vector<AsmMacroLexer::Token> tokens)
{
    Parser parser;
    auto tree = parser.parseTokens(tokens);
    return tree;
}

TEST_CASE("Parse Expressions")
{
    std::vector<AsmMacroLexer::Token> tokens = {
        {
            {0, 0},
            {0, 0},
            AsmMacroLexer::TokenType::VALUE,
            "1",
        },
        {
            {0, 0},
            {0, 0},
            AsmMacroLexer::TokenType::OPERATOR,
            "+",
        },
        {
            {0, 0},
            {0, 0},
            AsmMacroLexer::TokenType::VALUE,
            "2",
        },
    };
    // leaf: INT 1
    auto left = std::make_unique<Parser::AST>();
    left->begin = {0, 0};
    left->end = {0, 0};
    left->nodeType = Parser::NodeType::INT;
    left->intValue = 1;
    left->identifier = "";

    // leaf: INT 2
    auto right = std::make_unique<Parser::AST>();
    right->begin = {0, 0};
    right->end = {0, 0};
    right->nodeType = Parser::NodeType::INT;
    right->intValue = 2;
    right->identifier = "";

    // ADD
    auto add = std::make_unique<Parser::AST>();
    add->begin = {0, 0};
    add->end = {0, 0};
    add->nodeType = Parser::NodeType::ADD;
    add->intValue = 0;
    add->identifier = "";
    add->children.push_back(std::move(left));
    add->children.push_back(std::move(right));

    // LINE
    auto line = std::make_unique<Parser::AST>();
    line->begin = {0, 0};
    line->end = {0, 0};
    line->nodeType = Parser::NodeType::LINE;
    line->intValue = 0;
    line->identifier = "";
    line->children.push_back(std::move(add));

    // parent: PROGRAM
    auto program = std::make_unique<Parser::AST>();
    program->begin = {0, 0};
    program->end = {0, 0};
    program->nodeType = Parser::NodeType::PROGRAM;
    program->intValue = 0;
    program->identifier = "";
    program->children.push_back(std::move(line));

    REQUIRE(program == parseTokensHelper(tokens));
}