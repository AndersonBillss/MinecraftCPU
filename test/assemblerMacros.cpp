#include "catch_amalgamated.hpp"
#include "../src/assembler/macroSystem/macroSystem.hpp"
#include <iostream>

TEST_CASE("Variable storing maps work correctly"){
    MacroSystem program;
    program.setNumber("$test", 43);
    int value = std::get<int>(program.getVariable("$test").value);
    REQUIRE(value == 43);

    program.pushStack();
    program.setNumber("$test2", 22);
    value = std::get<int>(program.getVariable("$test2").value);
    REQUIRE(value == 22);
    value = std::get<int>(program.getVariable("$test").value);
    REQUIRE(value == 43);

    program.pushStack();
    program.setNumber("$test3", 87);
    value = std::get<int>(program.getVariable("$test3").value);
    REQUIRE(value == 87);
    value = std::get<int>(program.getVariable("$test").value);
    REQUIRE(value == 43);
}