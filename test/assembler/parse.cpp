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

std::unique_ptr<Parser::AST> parseWithoutSourceLocationHelper(std::string sourceCode)
{
    AsmMacroLexer lexer;
    Parser parser;
    auto tokens = lexer.tokenize(sourceCode);
    for (auto &t : tokens)
    {
        t.begin.line = 0;
        t.begin.column = 0;
        t.end.line = 0;
        t.end.column = 0;
    }
    auto tree = parser.parseTokens(tokens);
    return tree;
}

std::unique_ptr<Parser::AST> createNode(Parser::NodeType type)
{
    auto tree = std::make_unique<Parser::AST>();
    tree->begin = {0, 0};
    tree->end = {0, 0};
    tree->nodeType = type;
    tree->intValue = 0;
    tree->identifier = "";
    return tree;
}

TEST_CASE("Parse simple addition: 1 + 2")
{
    std::string sourceCode = "1 + 2";

    auto left = createNode(Parser::NodeType::INT);
    left->intValue = 1;

    auto right = createNode(Parser::NodeType::INT);
    right->intValue = 2;

    auto add = createNode(Parser::NodeType::ADD);
    add->children.push_back(std::move(left));
    add->children.push_back(std::move(right));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(add));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));
    //  PROGRAM
    //      LINE:
    //          ADD:
    //              INT: 1
    //              INT: 2

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse left-associtive addition: 1 + 2 + 3")
{
    std::string sourceCode = "1 + 2 + 3";

    auto term1 = createNode(Parser::NodeType::INT);
    term1->intValue = 1;

    auto term2 = createNode(Parser::NodeType::INT);
    term2->intValue = 2;

    auto term3 = createNode(Parser::NodeType::INT);
    term3->intValue = 3;

    auto add1 = createNode(Parser::NodeType::ADD);
    add1->children.push_back(std::move(term1));
    add1->children.push_back(std::move(term2));

    auto add2 = createNode(Parser::NodeType::ADD);
    add2->children.push_back(std::move(add1));
    add2->children.push_back(std::move(term3));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(add2));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));
    //  PROGRAM
    //      LINE:
    //          ADD:
    //              INT: 1
    //              ADD:
    //                  INT: 2
    //                  INT: 3

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse associativity with higher precedence operator: 1 + 2 * 3")
{
    std::string sourceCode = "1 + 2 * 3";

    auto term1 = createNode(Parser::NodeType::INT);
    term1->intValue = 1;

    auto factor1 = createNode(Parser::NodeType::INT);
    factor1->intValue = 2;

    auto factor2 = createNode(Parser::NodeType::INT);
    factor2->intValue = 3;

    auto multiply = createNode(Parser::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto add = createNode(Parser::NodeType::ADD);
    add->children.push_back(std::move(term1));
    add->children.push_back(std::move(multiply));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(add));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));
    //  PROGRAM
    //      LINE:
    //          ADD:
    //              INT: 1
    //              MULTIPLY:
    //                  INT: 2
    //                  INT: 3

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse associativity with higher and lower precedence operator: 1 + 2 * 3 + 4")
{
    std::string sourceCode = "1 + 2 * 3 + 4";

    auto term1 = createNode(Parser::NodeType::INT);
    term1->intValue = 1;

    auto factor1 = createNode(Parser::NodeType::INT);
    factor1->intValue = 2;

    auto factor2 = createNode(Parser::NodeType::INT);
    factor2->intValue = 3;

    auto term2 = createNode(Parser::NodeType::INT);
    term2->intValue = 4;

    auto multiply = createNode(Parser::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto add1 = createNode(Parser::NodeType::ADD);
    add1->children.push_back(std::move(term1));
    add1->children.push_back(std::move(multiply));

    auto add2 = createNode(Parser::NodeType::ADD);
    add2->children.push_back(std::move(add1));
    add2->children.push_back(std::move(term2));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(add2));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));
    //  PROGRAM
    //      LINE:
    //          ADD:
    //              ADD:
    //                  INT: 1
    //                  MULTIPLY:
    //                      INT: 2
    //                      INT: 3
    //              INT: 4
}

TEST_CASE("Parse concatenation expression: 1 2")
{
    std::string sourceCode = "1 2";

    auto concatOperand1 = createNode(Parser::NodeType::INT);
    concatOperand1->intValue = 1;

    auto concatOperand2 = createNode(Parser::NodeType::INT);
    concatOperand2->intValue = 2;

    auto concat = createNode(Parser::NodeType::CONCAT);
    concat->children.push_back(std::move(concatOperand1));
    concat->children.push_back(std::move(concatOperand2));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(concat));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse concatenation expression with operators: 1 + 2 3")
{
    std::string sourceCode = "1 + 2 3";

    auto term1 = createNode(Parser::NodeType::INT);
    term1->intValue = 1;
    auto term2 = createNode(Parser::NodeType::INT);
    term2->intValue = 2;

    auto concatOperand1 = createNode(Parser::NodeType::ADD);
    concatOperand1->children.push_back(std::move(term1));
    concatOperand1->children.push_back(std::move(term2));

    auto concatOperand2 = createNode(Parser::NodeType::INT);
    concatOperand2->intValue = 3;

    auto concat = createNode(Parser::NodeType::CONCAT);
    concat->children.push_back(std::move(concatOperand1));
    concat->children.push_back(std::move(concatOperand2));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(concat));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse single operand expression: 1")
{
    std::string sourceCode = "1";

    auto operand = createNode(Parser::NodeType::INT);
    operand->intValue = 1;

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(operand));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse multi-line expression")
{
    std::string sourceCode = R"(
        1 + 2
        2 + 3
    )";

    auto ex1Term1 = createNode(Parser::NodeType::INT);
    ex1Term1->intValue = 1;

    auto ex1Term2 = createNode(Parser::NodeType::INT);
    ex1Term2->intValue = 2;

    auto add1 = createNode(Parser::NodeType::ADD);
    add1->children.push_back(std::move(ex1Term1));
    add1->children.push_back(std::move(ex1Term2));

    auto line1 = createNode(Parser::NodeType::LINE);
    line1->children.push_back(std::move(add1));

    auto ex2Term1 = createNode(Parser::NodeType::INT);
    ex2Term1->intValue = 2;

    auto ex2Term2 = createNode(Parser::NodeType::INT);
    ex2Term2->intValue = 3;

    auto add2 = createNode(Parser::NodeType::ADD);
    add2->children.push_back(std::move(ex2Term1));
    add2->children.push_back(std::move(ex2Term2));

    auto line2 = createNode(Parser::NodeType::LINE);
    line2->children.push_back(std::move(add2));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line1));
    program->children.push_back(std::move(line2));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse single values within parentheses")
{
    std::string sourceCode = "(1)";

    auto operand = createNode(Parser::NodeType::INT);
    operand->intValue = 1;

    auto block = createNode(Parser::NodeType::BLOCK);
    block->children.push_back(std::move(operand));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(block));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse expression within parentheses")
{
    std::string sourceCode = "(1 + 2 * 3)";

    auto term1 = createNode(Parser::NodeType::INT);
    term1->intValue = 1;

    auto factor1 = createNode(Parser::NodeType::INT);
    factor1->intValue = 2;

    auto factor2 = createNode(Parser::NodeType::INT);
    factor2->intValue = 3;

    auto multiply = createNode(Parser::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto add = createNode(Parser::NodeType::ADD);
    add->children.push_back(std::move(term1));
    add->children.push_back(std::move(multiply));

    auto block = createNode(Parser::NodeType::BLOCK);
    block->children.push_back(std::move(add));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(block));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse expression with lower precedence operators in parentheses")
{
    std::string sourceCode = "3 * (1 + 2)";

    auto factor1 = createNode(Parser::NodeType::INT);
    factor1->intValue = 3;

    auto term1 = createNode(Parser::NodeType::INT);
    term1->intValue = 1;

    auto term2 = createNode(Parser::NodeType::INT);
    term2->intValue = 2;

    auto add = createNode(Parser::NodeType::ADD);
    add->children.push_back(std::move(term1));
    add->children.push_back(std::move(term2));

    auto factor2 = createNode(Parser::NodeType::BLOCK);
    factor2->children.push_back(std::move(add));

    auto multiply = createNode(Parser::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(multiply));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse variable assigning")
{
    std::string sourceCode = "$a = 4";

    auto variable = createNode(Parser::NodeType::IDENTIFIER);
    variable->identifier = "$a";

    auto integer = createNode(Parser::NodeType::INT);
    integer->intValue = 4;

    auto assignmentNode = createNode(Parser::NodeType::ASSIGNMENT);
    assignmentNode->children.push_back(std::move(variable));
    assignmentNode->children.push_back(std::move(integer));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(assignmentNode));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse variable assigning multiple lines")
{
    std::string sourceCode = R"(
    $a = 4
    $b = 5
    )";

    auto variable1 = createNode(Parser::NodeType::IDENTIFIER);
    variable1->identifier = "$a";

    auto integer1 = createNode(Parser::NodeType::INT);
    integer1->intValue = 4;

    auto assignmentNode1 = createNode(Parser::NodeType::ASSIGNMENT);
    assignmentNode1->children.push_back(std::move(variable1));
    assignmentNode1->children.push_back(std::move(integer1));

    auto line1 = createNode(Parser::NodeType::LINE);
    line1->children.push_back(std::move(assignmentNode1));

    auto variable2 = createNode(Parser::NodeType::IDENTIFIER);
    variable2->identifier = "$b";

    auto integer2 = createNode(Parser::NodeType::INT);
    integer2->intValue = 5;

    auto assignmentNode2 = createNode(Parser::NodeType::ASSIGNMENT);
    assignmentNode2->children.push_back(std::move(variable2));
    assignmentNode2->children.push_back(std::move(integer2));

    auto line2 = createNode(Parser::NodeType::LINE);
    line2->children.push_back(std::move(assignmentNode2));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line1));
    program->children.push_back(std::move(line2));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse expressions across multiple lines")
{
    std::string sourceCode = R"(
    2      
    +3

    * 4
    )";

    auto factor1 = createNode(Parser::NodeType::INT);
    factor1->intValue = 3;

    auto factor2 = createNode(Parser::NodeType::INT);
    factor2->intValue = 4;

    auto multiply = createNode(Parser::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto term1 = createNode(Parser::NodeType::INT);
    term1->intValue = 2;

    auto add = createNode(Parser::NodeType::ADD);
    add->children.push_back(std::move(term1));
    add->children.push_back(std::move(multiply));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(add));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Test")
{
    std::string sourceCode = R"(
        LDI R1 (
           $test = 2
           1 + $test
        )
    )";
    auto assignedVariable = createNode(Parser::NodeType::IDENTIFIER);
    assignedVariable->identifier = "$test";

    auto assignedInt = createNode(Parser::NodeType::INT);
    assignedInt->intValue = 2;

    auto assignmentNode = createNode(Parser::NodeType::ASSIGNMENT);
    assignmentNode->children.push_back(std::move(assignedVariable));
    assignmentNode->children.push_back(std::move(assignedInt));

    auto blockLine1 = createNode(Parser::NodeType::LINE);
    blockLine1->children.push_back(std::move(assignmentNode));

    auto addInt = createNode(Parser::NodeType::INT);
    addInt->intValue = 1;

    auto addVariable = createNode(Parser::NodeType::IDENTIFIER);
    addVariable->identifier = "$test";

    auto addNode = createNode(Parser::NodeType::ADD);
    addNode->children.push_back(std::move(addInt));
    addNode->children.push_back(std::move(addVariable));

    auto blockLine2 = createNode(Parser::NodeType::LINE);
    blockLine2->children.push_back(std::move(addNode));

    auto blockNode = createNode(Parser::NodeType::BLOCK);
    blockNode->children.push_back(std::move(blockLine1));
    blockNode->children.push_back(std::move(blockLine2));

    auto string1 = createNode(Parser::NodeType::STRING);
    string1->identifier = "LDI";

    auto string2 = createNode(Parser::NodeType::STRING);
    string2->identifier = "R1";

    auto concat1 = createNode(Parser::NodeType::CONCAT);
    concat1->children.push_back(std::move(string1));
    concat1->children.push_back(std::move(string2));

    auto concat2 = createNode(Parser::NodeType::CONCAT);
    concat2->children.push_back(std::move(concat1));
    concat2->children.push_back(std::move(blockNode));

    auto line = createNode(Parser::NodeType::LINE);
    line->children.push_back(std::move(concat2));

    auto program = createNode(Parser::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}