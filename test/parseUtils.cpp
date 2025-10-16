#include <string>

#include "catch_amalgamated.hpp"
#include "../src/utils/parseUtils.hpp"

TEST_CASE("Parse hexadecimal integers")
{
    std::string hexString = "0x3";
    int expected = 0x3;
    REQUIRE(ParseUtils::parseInt(hexString, 32) == expected);
}