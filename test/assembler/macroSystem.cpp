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

TEST_CASE("MacroSystem evaluates function with two parameters", "[macroSystem]")
{
    std::string sourceCode = R"(
    $mul = $a $b => $a * $b
    $mul 2 3
    )";
    std::string result = "6\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates concatenated functions", "[macroSystem]")
{
    std::string sourceCode = R"(
    $mul = $a $b => $a * $b
    LDI R1 $mul 2 3
    )";
    std::string result = "LDI R1 6\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates functions with variable arguments", "[macroSystem]")
{
    std::string sourceCode = R"(
    $mul = $a $b => $a * $b
    $varA = 2
    $varB = 3
    $mul $varA $varB
    )";
    std::string result = "6\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates functions with expressions on the right-hand side", "[macroSystem]")
{
    std::string sourceCode = R"(
    $mul = $a $b => $a * $b
    $varA = 2
    $varB = 3
    $mul $varA $varB + 100
    )";
    std::string result = "106\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates functions with block arguments", "[macroSystem]")
{
    std::string sourceCode = R"(
    $mul = $a $b => $a * $b
    $varA = 2
    $varB = 3
    $mul $varA ($varB + 100)
    )";
    std::string result = "206\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}

TEST_CASE("MacroSystem evaluates lines after returning from function", "[macroSystem]")
{
    std::string sourceCode = R"(
    $mul = $a $b => $a * $b
    $mul 2 3
    LDI R1 1
    )";
    std::string result = "6\nLDI R1 1\n";
    REQUIRE(evaluateMacroHelper(sourceCode) == result);
}