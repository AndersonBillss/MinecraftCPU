#include "ASTUtils.hpp"
#include <unordered_map>

std::string stringifyTree(const AST::Node &tree, std::string tabs)
{
    std::unordered_map<AST::NodeType, std::string> nodeToString{
        {AST::NodeType::PROGRAM, "PROGRAM"},
        {AST::NodeType::BLOCK, "BLOCK"},
        {AST::NodeType::LINE, "LINE"},

        {AST::NodeType::ADD, "ADD"},
        {AST::NodeType::SUB, "SUB"},
        {AST::NodeType::OR, "OR"},
        {AST::NodeType::AND, "AND"},
        {AST::NodeType::XOR, "XOR"},
        {AST::NodeType::MULTIPLY, "MULTIPLY"},
        {AST::NodeType::DIVIDE, "DIVIDE"},
        {AST::NodeType::CONCAT, "CONCAT"},

        {AST::NodeType::ASSIGNMENT, "ASSIGNMENT"},
        {AST::NodeType::FUNCTION, "FUNCTION"},
        {AST::NodeType::PARAMETER_LIST, "PARAMETER_LIST"},
        {AST::NodeType::CALL, "CALL"},
        {AST::NodeType::ARGUMENT_LIST, "ARGUMENT_LIST"},

        {AST::NodeType::IDENTIFIER, "IDENTIFIER"},
        {AST::NodeType::STRING, "STRING"},
        {AST::NodeType::INT, "INT"},
        {AST::NodeType::UNDEFINED, "UNDEFINED"},
    };
    std::string stringifiedNodeType = "?????";
    auto it = nodeToString.find(tree.nodeType);
    if (it != nodeToString.end())
    {
        stringifiedNodeType = it->second;
    }
    std::string stringifiedNumber = "";
    if (tree.nodeType == AST::NodeType::INT)
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

std::unique_ptr<AST::Node> createNode(AST::NodeType type)
{
    auto tree = std::make_unique<AST::Node>();
    tree->begin = {0, 0};
    tree->end = {0, 0};
    tree->nodeType = type;
    tree->intValue = 0;
    tree->identifier = "";
    return tree;
}