#include "catch_amalgamated.hpp"
#include "../src/assembler/assembler.hpp"
#include <string>

TEST_CASE("Assembler compiles basic assembly") {
    std::string program = "LDI R0 1";
    REQUIRE(Assembler::compile(program) == "00000001 00000000 00000001");
}