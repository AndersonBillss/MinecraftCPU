#include "catch_amalgamated.hpp"
#include "../src/assembler/macroSystem/macroSystem.hpp"
#include <iostream>

TEST_CASE("Variable storing maps work correctly"){
    MacroSystem program;
    program.setValue("$test", 43);
    int value = program.getValue("$test");
    REQUIRE(value == 43);
}