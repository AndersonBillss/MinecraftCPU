#include "catch_amalgamated.hpp"
#include "../../src/assembler/macroSystem/lexer.hpp"
#include "../../src/assembler/macroSystem/parser.hpp"
#include <string>

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

namespace AST
{
    inline std::ostream &operator<<(std::ostream &os,
                                    const std::unique_ptr<AST::Node> &tree)
    {
        return os << stringifyTree(*tree, "");
    }

    inline bool operator==(const std::unique_ptr<AST::Node> &a, const std::unique_ptr<AST::Node> &b)
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
}

std::unique_ptr<AST::Node> parseWithoutSourceLocationHelper(std::string sourceCode)
{
    Lexer lexer;
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

TEST_CASE("Parse simple addition: 1 + 2")
{
    std::string sourceCode = "1 + 2";

    auto left = createNode(AST::NodeType::INT);
    left->intValue = 1;

    auto right = createNode(AST::NodeType::INT);
    right->intValue = 2;

    auto add = createNode(AST::NodeType::ADD);
    add->children.push_back(std::move(left));
    add->children.push_back(std::move(right));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(add));

    auto program = createNode(AST::NodeType::PROGRAM);
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

    auto term1 = createNode(AST::NodeType::INT);
    term1->intValue = 1;

    auto term2 = createNode(AST::NodeType::INT);
    term2->intValue = 2;

    auto term3 = createNode(AST::NodeType::INT);
    term3->intValue = 3;

    auto add1 = createNode(AST::NodeType::ADD);
    add1->children.push_back(std::move(term1));
    add1->children.push_back(std::move(term2));

    auto add2 = createNode(AST::NodeType::ADD);
    add2->children.push_back(std::move(add1));
    add2->children.push_back(std::move(term3));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(add2));

    auto program = createNode(AST::NodeType::PROGRAM);
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

    auto term1 = createNode(AST::NodeType::INT);
    term1->intValue = 1;

    auto factor1 = createNode(AST::NodeType::INT);
    factor1->intValue = 2;

    auto factor2 = createNode(AST::NodeType::INT);
    factor2->intValue = 3;

    auto multiply = createNode(AST::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto add = createNode(AST::NodeType::ADD);
    add->children.push_back(std::move(term1));
    add->children.push_back(std::move(multiply));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(add));

    auto program = createNode(AST::NodeType::PROGRAM);
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

    auto term1 = createNode(AST::NodeType::INT);
    term1->intValue = 1;

    auto factor1 = createNode(AST::NodeType::INT);
    factor1->intValue = 2;

    auto factor2 = createNode(AST::NodeType::INT);
    factor2->intValue = 3;

    auto term2 = createNode(AST::NodeType::INT);
    term2->intValue = 4;

    auto multiply = createNode(AST::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto add1 = createNode(AST::NodeType::ADD);
    add1->children.push_back(std::move(term1));
    add1->children.push_back(std::move(multiply));

    auto add2 = createNode(AST::NodeType::ADD);
    add2->children.push_back(std::move(add1));
    add2->children.push_back(std::move(term2));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(add2));

    auto program = createNode(AST::NodeType::PROGRAM);
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

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse concatenation expression: 1 2")
{
    std::string sourceCode = "1 2";

    auto concatOperand1 = createNode(AST::NodeType::INT);
    concatOperand1->intValue = 1;

    auto concatOperand2 = createNode(AST::NodeType::INT);
    concatOperand2->intValue = 2;

    auto concat = createNode(AST::NodeType::CONCAT);
    concat->children.push_back(std::move(concatOperand1));
    concat->children.push_back(std::move(concatOperand2));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(concat));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse concatenation expression with operators: 1 + 2 3")
{
    std::string sourceCode = "1 + 2 3";

    auto term1 = createNode(AST::NodeType::INT);
    term1->intValue = 1;
    auto term2 = createNode(AST::NodeType::INT);
    term2->intValue = 2;

    auto concatOperand1 = createNode(AST::NodeType::ADD);
    concatOperand1->children.push_back(std::move(term1));
    concatOperand1->children.push_back(std::move(term2));

    auto concatOperand2 = createNode(AST::NodeType::INT);
    concatOperand2->intValue = 3;

    auto concat = createNode(AST::NodeType::CONCAT);
    concat->children.push_back(std::move(concatOperand1));
    concat->children.push_back(std::move(concatOperand2));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(concat));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse single operand expression: 1")
{
    std::string sourceCode = "1";

    auto operand = createNode(AST::NodeType::INT);
    operand->intValue = 1;

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(operand));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse multi-line expression")
{
    std::string sourceCode = R"(
        1 + 2
        2 + 3
    )";

    auto ex1Term1 = createNode(AST::NodeType::INT);
    ex1Term1->intValue = 1;

    auto ex1Term2 = createNode(AST::NodeType::INT);
    ex1Term2->intValue = 2;

    auto add1 = createNode(AST::NodeType::ADD);
    add1->children.push_back(std::move(ex1Term1));
    add1->children.push_back(std::move(ex1Term2));

    auto line1 = createNode(AST::NodeType::LINE);
    line1->children.push_back(std::move(add1));

    auto ex2Term1 = createNode(AST::NodeType::INT);
    ex2Term1->intValue = 2;

    auto ex2Term2 = createNode(AST::NodeType::INT);
    ex2Term2->intValue = 3;

    auto add2 = createNode(AST::NodeType::ADD);
    add2->children.push_back(std::move(ex2Term1));
    add2->children.push_back(std::move(ex2Term2));

    auto line2 = createNode(AST::NodeType::LINE);
    line2->children.push_back(std::move(add2));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line1));
    program->children.push_back(std::move(line2));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse single values within parentheses")
{
    std::string sourceCode = "(1)";

    auto operand = createNode(AST::NodeType::INT);
    operand->intValue = 1;

    auto block = createNode(AST::NodeType::BLOCK);
    block->children.push_back(std::move(operand));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(block));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse expression within parentheses")
{
    std::string sourceCode = "(1 + 2 * 3)";

    auto term1 = createNode(AST::NodeType::INT);
    term1->intValue = 1;

    auto factor1 = createNode(AST::NodeType::INT);
    factor1->intValue = 2;

    auto factor2 = createNode(AST::NodeType::INT);
    factor2->intValue = 3;

    auto multiply = createNode(AST::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto add = createNode(AST::NodeType::ADD);
    add->children.push_back(std::move(term1));
    add->children.push_back(std::move(multiply));

    auto block = createNode(AST::NodeType::BLOCK);
    block->children.push_back(std::move(add));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(block));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse expression with lower precedence operators in parentheses")
{
    std::string sourceCode = "3 * (1 + 2)";

    auto factor1 = createNode(AST::NodeType::INT);
    factor1->intValue = 3;

    auto term1 = createNode(AST::NodeType::INT);
    term1->intValue = 1;

    auto term2 = createNode(AST::NodeType::INT);
    term2->intValue = 2;

    auto add = createNode(AST::NodeType::ADD);
    add->children.push_back(std::move(term1));
    add->children.push_back(std::move(term2));

    auto factor2 = createNode(AST::NodeType::BLOCK);
    factor2->children.push_back(std::move(add));

    auto multiply = createNode(AST::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(multiply));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse variable assigning")
{
    std::string sourceCode = "$a = 4";

    auto variable = createNode(AST::NodeType::IDENTIFIER);
    variable->identifier = "$a";

    auto integer = createNode(AST::NodeType::INT);
    integer->intValue = 4;

    auto assignmentNode = createNode(AST::NodeType::ASSIGNMENT);
    assignmentNode->children.push_back(std::move(variable));
    assignmentNode->children.push_back(std::move(integer));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(assignmentNode));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse variable assigning multiple lines")
{
    std::string sourceCode = R"(
    $a = 4
    $b = 5
    )";

    auto variable1 = createNode(AST::NodeType::IDENTIFIER);
    variable1->identifier = "$a";

    auto integer1 = createNode(AST::NodeType::INT);
    integer1->intValue = 4;

    auto assignmentNode1 = createNode(AST::NodeType::ASSIGNMENT);
    assignmentNode1->children.push_back(std::move(variable1));
    assignmentNode1->children.push_back(std::move(integer1));

    auto line1 = createNode(AST::NodeType::LINE);
    line1->children.push_back(std::move(assignmentNode1));

    auto variable2 = createNode(AST::NodeType::IDENTIFIER);
    variable2->identifier = "$b";

    auto integer2 = createNode(AST::NodeType::INT);
    integer2->intValue = 5;

    auto assignmentNode2 = createNode(AST::NodeType::ASSIGNMENT);
    assignmentNode2->children.push_back(std::move(variable2));
    assignmentNode2->children.push_back(std::move(integer2));

    auto line2 = createNode(AST::NodeType::LINE);
    line2->children.push_back(std::move(assignmentNode2));

    auto program = createNode(AST::NodeType::PROGRAM);
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

    auto factor1 = createNode(AST::NodeType::INT);
    factor1->intValue = 3;

    auto factor2 = createNode(AST::NodeType::INT);
    factor2->intValue = 4;

    auto multiply = createNode(AST::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(factor1));
    multiply->children.push_back(std::move(factor2));

    auto term1 = createNode(AST::NodeType::INT);
    term1->intValue = 2;

    auto add = createNode(AST::NodeType::ADD);
    add->children.push_back(std::move(term1));
    add->children.push_back(std::move(multiply));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(add));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse multiple lines in block node")
{
    std::string sourceCode = R"(
        LDI R1 (
           $test = 2
           1 + $test
        )
    )";
    auto assignedVariable = createNode(AST::NodeType::IDENTIFIER);
    assignedVariable->identifier = "$test";

    auto assignedInt = createNode(AST::NodeType::INT);
    assignedInt->intValue = 2;

    auto assignmentNode = createNode(AST::NodeType::ASSIGNMENT);
    assignmentNode->children.push_back(std::move(assignedVariable));
    assignmentNode->children.push_back(std::move(assignedInt));

    auto blockLine1 = createNode(AST::NodeType::LINE);
    blockLine1->children.push_back(std::move(assignmentNode));

    auto addInt = createNode(AST::NodeType::INT);
    addInt->intValue = 1;

    auto addVariable = createNode(AST::NodeType::IDENTIFIER);
    addVariable->identifier = "$test";

    auto addNode = createNode(AST::NodeType::ADD);
    addNode->children.push_back(std::move(addInt));
    addNode->children.push_back(std::move(addVariable));

    auto blockLine2 = createNode(AST::NodeType::LINE);
    blockLine2->children.push_back(std::move(addNode));

    auto blockNode = createNode(AST::NodeType::BLOCK);
    blockNode->children.push_back(std::move(blockLine1));
    blockNode->children.push_back(std::move(blockLine2));

    auto string1 = createNode(AST::NodeType::STRING);
    string1->identifier = "LDI";

    auto string2 = createNode(AST::NodeType::STRING);
    string2->identifier = "R1";

    auto concat1 = createNode(AST::NodeType::CONCAT);
    concat1->children.push_back(std::move(string1));
    concat1->children.push_back(std::move(string2));

    auto concat2 = createNode(AST::NodeType::CONCAT);
    concat2->children.push_back(std::move(concat1));
    concat2->children.push_back(std::move(blockNode));

    auto line = createNode(AST::NodeType::LINE);
    line->children.push_back(std::move(concat2));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse function")
{
    std::string sourceCode = R"(
        $myFn = $1 $2 => $1 * $2
    )";

    auto param1 = createNode(AST::NodeType::IDENTIFIER);
    param1->identifier = "$1";

    auto param2 = createNode(AST::NodeType::IDENTIFIER);
    param2->identifier = "$2";

    auto paramList = createNode(AST::NodeType::PARAMETER_LIST);
    paramList->children.push_back(std::move(param1));
    paramList->children.push_back(std::move(param2));

    auto product1 = createNode(AST::NodeType::IDENTIFIER);
    product1->identifier = "$1";

    auto product2 = createNode(AST::NodeType::IDENTIFIER);
    product2->identifier = "$2";

    auto multiply = createNode(AST::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(product1));
    multiply->children.push_back(std::move(product2));

    auto functionIdentifier = createNode(AST::NodeType::IDENTIFIER);
    functionIdentifier->identifier = "$myFn";

    auto function = createNode(AST::NodeType::FUNCTION);
    function->children.push_back(std::move(paramList));
    function->children.push_back(std::move(multiply));

    auto assignmentNode = createNode(AST::NodeType::ASSIGNMENT);
    assignmentNode->children.push_back(std::move(functionIdentifier));
    assignmentNode->children.push_back(std::move(function));

    auto lineNode = createNode(AST::NodeType::LINE);
    lineNode->children.push_back(std::move(assignmentNode));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(lineNode));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}

TEST_CASE("Parse function calls")
{
    std::string sourceCode = R"(
        $myFn = $1 $2 => $1 * $2
        LDI R1 $myFn 1 2
    )";

    auto param1 = createNode(AST::NodeType::IDENTIFIER);
    param1->identifier = "$1";

    auto param2 = createNode(AST::NodeType::IDENTIFIER);
    param2->identifier = "$2";

    auto paramList = createNode(AST::NodeType::PARAMETER_LIST);
    paramList->children.push_back(std::move(param1));
    paramList->children.push_back(std::move(param2));

    auto product1 = createNode(AST::NodeType::IDENTIFIER);
    product1->identifier = "$1";

    auto product2 = createNode(AST::NodeType::IDENTIFIER);
    product2->identifier = "$2";

    auto multiply = createNode(AST::NodeType::MULTIPLY);
    multiply->children.push_back(std::move(product1));
    multiply->children.push_back(std::move(product2));

    auto functionNode = createNode(AST::NodeType::FUNCTION);
    functionNode->children.push_back(std::move(paramList));
    functionNode->children.push_back(std::move(multiply));

    auto functionName = createNode(AST::NodeType::IDENTIFIER);
    functionName->identifier = "$myFn";

    auto functionAssignment = createNode(AST::NodeType::ASSIGNMENT);
    functionAssignment->children.push_back(std::move(functionName));
    functionAssignment->children.push_back(std::move(functionNode));

    auto line1 = createNode(AST::NodeType::LINE);
    line1->children.push_back(std::move(functionAssignment));

    auto arg1 = createNode(AST::NodeType::INT);
    arg1->intValue = 1;

    auto arg2 = createNode(AST::NodeType::INT);
    arg2->intValue = 2;

    auto functionCallIdentifier = createNode(AST::NodeType::IDENTIFIER);
    functionCallIdentifier->identifier = "$myFn";

    auto argList = createNode(AST::NodeType::ARGUMENT_LIST);
    argList->children.push_back(std::move(arg1));
    argList->children.push_back(std::move(arg2));

    auto functionCallNode = createNode(AST::NodeType::CALL);
    functionCallNode->children.push_back(std::move(functionCallIdentifier));
    functionCallNode->children.push_back(std::move(argList));

    auto string1 = createNode(AST::NodeType::STRING);
    string1->identifier = "LDI";

    auto string2 = createNode(AST::NodeType::STRING);
    string2->identifier = "R1";

    auto concat1 = createNode(AST::NodeType::CONCAT);
    concat1->children.push_back(std::move(string1));
    concat1->children.push_back(std::move(string2));

    auto concat2 = createNode(AST::NodeType::CONCAT);
    concat2->children.push_back(std::move(concat1));
    concat2->children.push_back(std::move(functionCallNode));

    auto line2 = createNode(AST::NodeType::LINE);
    line2->children.push_back(std::move(concat2));

    auto program = createNode(AST::NodeType::PROGRAM);
    program->children.push_back(std::move(line1));
    program->children.push_back(std::move(line2));

    REQUIRE(program == parseWithoutSourceLocationHelper(sourceCode));
}