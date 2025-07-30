#include "catch_amalgamated.hpp"
#include "../src/assembler/macroSystem/macroSystem.hpp"
#include "../src/assembler/macroSystem/lexer.hpp"
#include "../src/utils/runtimeError.hpp"
#include <iostream>

TEST_CASE("Variables can be stored across stacks")
{
    MacroSystem program;
    program.setNumber("$test1", 43);
    int value = std::get<int>(program.getVariable("$test1").value);
    REQUIRE(value == 43);

    program.pushStack();
    program.setNumber("$test2", 22);
    value = std::get<int>(program.getVariable("$test2").value);
    REQUIRE(value == 22);
    value = std::get<int>(program.getVariable("$test1").value);
    REQUIRE(value == 43);

    program.pushStack();
    program.setNumber("$test3", 87);
    value = std::get<int>(program.getVariable("$test3").value);
    REQUIRE(value == 87);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    value = std::get<int>(program.getVariable("$test1").value);
    REQUIRE(value == 43);

    program.popStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    value = std::get<int>(program.getVariable("$test2").value);
    REQUIRE(value == 22);
    value = std::get<int>(program.getVariable("$test1").value);
    REQUIRE(value == 43);

    program.popStack();
    REQUIRE_THROWS_AS(program.getVariable("$test3"), RuntimeError);
    REQUIRE_THROWS_AS(program.getVariable("$test2"), RuntimeError);
    value = std::get<int>(program.getVariable("$test1").value);
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
}