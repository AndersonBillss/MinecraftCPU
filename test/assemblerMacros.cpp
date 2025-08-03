#include "catch_amalgamated.hpp"
#include "../src/assembler/macroSystem/macroSystem.hpp"
#include "../src/assembler/macroSystem/lexer.hpp"
#include "../src/utils/runtimeError.hpp"
#include <iostream>

TEST_CASE("Variables can be stored across stacks")
{
    MacroSystem program;
    program.setNumber("$test1", 43);
    int value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);

    program.pushStack();
    program.setNumber("$test2", 22);
    value = std::get<unsigned int>(program.getVariable("$test2"));
    REQUIRE(value == 22);
    value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);

    program.pushStack();
    program.setNumber("$test3", 87);
    value = std::get<unsigned int>(program.getVariable("$test3"));
    REQUIRE(value == 87);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);

    program.popStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    value = std::get<unsigned int>(program.getVariable("$test2"));
    REQUIRE(value == 22);
    value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);

    program.popStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    value = std::get<unsigned int>(program.getVariable("$test1"));
    REQUIRE(value == 43);
}

TEST_CASE("Tokenization logic works correctly")
{
    std::string sourceCode = R"(
        LDI R1 1
    )";
    std::vector<std::string> expected = {
        "LDI",
        "R1",
        "1"
    };
    REQUIRE(AsmMacroLexer::tokenize(sourceCode) == expected);


    sourceCode = R"(
        $test = 10
        LDI R1 1
        $test2 = (LDI R1 $test)
    )";
    expected = {
        "$test",
        "=",
        "10",
        "\n",
        "LDI",
        "R1",
        "1",
        "\n",
        "$test2",
        "=",
        "(LDI R1 $test)"
    };
    REQUIRE(AsmMacroLexer::tokenize(sourceCode) == expected);


    sourceCode = R"(
        $test=10 +
        LDI R1 1

        $test2 = (LDI R1 ($test * 2))
        NOP

        NOP

    )";
    expected = {
        "$test",
        "=",
        "10",
        "+",
        "\n",
        "LDI",
        "R1",
        "1",
        "\n",
        "$test2",
        "=",
        "(LDI R1 ($test * 2))",
        "\n",
        "NOP",
        "\n",
        "NOP"
    };
    REQUIRE(AsmMacroLexer::tokenize(sourceCode) == expected);


    sourceCode = R"(
        $test=10 +
        LDI R1 1

        $test2 = (LDI R1 ($test * 2))
        NOP

    .specialFunction:
        NOP

    )";
    expected = {
        "$test",
        "=",
        "10",
        "+",
        "\n",
        "LDI",
        "R1",
        "1",
        "\n",
        "$test2",
        "=",
        "(LDI R1 ($test * 2))",
        "\n",
        "NOP",
        "\n",
        ".specialFunction:",
        "\n",
        "NOP"
    };
    REQUIRE(AsmMacroLexer::tokenize(sourceCode) == expected);
}

TEST_CASE("Evaluate variables and symbols")
{
    MacroSystem m;
    std::string sourceCode = R"(
        $sample = 10
        $sample
    )";
    std::string expected = "10\n";
    REQUIRE(m.evaluate(sourceCode) == expected);
}