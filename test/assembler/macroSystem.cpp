#include "catch_amalgamated.hpp"
#include "../../src/assembler/macroSystem/MacroSystem.hpp"
#include "../../src/assembler/macroSystem/lexer.hpp"
#include "../../src/assembler/macroSystem/parser.hpp"
#include "../../src/utils/runtimeError.hpp"
#include "ASTUtils.hpp"
#include <iostream>

std::string evaluateMacroHelper(std::string &sourceCode)
{
    auto lexer = Lexer();
    auto tokens = lexer.tokenize(sourceCode);
    auto parser = Parser();
    auto ast = parser.parseTokens(tokens);

    std::string result = "";
    auto macroSystem = MacroSystem(ast.get());
    while (!macroSystem.done())
        result += macroSystem.getLine() + "\n";

    return result;
}

TEST_CASE("MacroSystem stores variables across stacks", "[macroSystem]")
{
    std::unique_ptr<AST::Node> node = std::make_unique<AST::Node>(AST::Node{
        begin : {0, 0},
        end : {0, 0},
        nodeType : AST::NodeType::PROGRAM,
        {},
        "",
        0
    });
    MacroSystem program(node.get());

    auto val1 = createNode(AST::NodeType::INT);
    val1->intValue = 43;
    program.setVariable("$test1", val1.get());
    REQUIRE(program.getVariable("$test1")->intValue == 43);

    program.pushVariableStack();
    auto val2 = createNode(AST::NodeType::INT);
    val2->intValue = 22;
    program.setVariable("$test2", val2.get());
    REQUIRE(program.getVariable("$test2")->intValue == 22);
    REQUIRE(program.getVariable("$test1")->intValue == 43);

    program.pushVariableStack();
    auto val3 = createNode(AST::NodeType::INT);
    val3->intValue = 87;
    program.setVariable("$test3", val3.get());
    REQUIRE(program.getVariable("$test3")->intValue == 87);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    REQUIRE(program.getVariable("$test1")->intValue == 43);

    program.popVariableStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    REQUIRE(program.getVariable("$test2")->intValue == 22);
    REQUIRE(program.getVariable("$test1")->intValue == 43);

    program.popVariableStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    REQUIRE(program.getVariable("$test1")->intValue == 43);
}

TEST_CASE("MacroSystem evaluates single variabeles", "[macroSystem]")
{
    std::string sourceCode = "1";
    std::string result = "1\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates simple expressions", "[macroSystem]")
{
    std::string sourceCode = "1 + 2";
    std::string result = "3\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates right-associative expressions", "[macroSystem]")
{
    std::string sourceCode = "1 + 2 * 3";
    std::string result = "7\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates expressions mixed with strings", "[macroSystem]")
{
    std::string sourceCode = "LDI R1 1 + 2 * 3 TEST";
    std::string result = "LDI R1 7 TEST\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates multiple lines", "[macroSystem]")
{
    std::string sourceCode = R"(
        1 + 2
        2 + 3
    )";
    std::string result = "3\n5\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem stores variables", "[macroSystem]")
{
    std::string sourceCode = R"(
        $hello = 1
        $hello
    )";
    std::string result = "1\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates blocks", "[macroSystem]")
{
    std::string sourceCode = R"(
        (1)
    )";
    std::string result = "1\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates multi-line blocks", "[macroSystem]")
{
    std::string sourceCode = R"(
    (
        1
        2
    )
    )";
    std::string result = "1\n2\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates multi-line blocks with expressions", "[macroSystem]")
{
    std::string sourceCode = R"(
    (
        1 + 2
        2
    )
    )";
    std::string result = "3\n2\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates multi-line blocks with expressions and variables", "[macroSystem]")
{
    std::string sourceCode = R"(
    (
        $testNum = 50
        1 
        2 + $testNum
    )
    )";
    std::string result = "1\n52\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates blocks in concat expressions", "[macroSystem]")
{
    std::string sourceCode = R"(
    LDI R1 (
        $testNum = 50
        2 + $testNum
    )
    )";
    std::string result = "LDI R1 52\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates outside concat expressions", "[macroSystem]")
{
    std::string sourceCode = R"(
    LDI R1 
    (
        $testNum = 50
        2 + $testNum
    )
    )";
    std::string result = "LDI R1\n52\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates function with one parameter", "[macroSystem]")
{
    std::string sourceCode = R"(
    $increment = $a => $a + 1
    $increment 3
    )";
    std::string result = "4\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}