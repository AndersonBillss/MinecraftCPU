#include <string>

#include "catch_amalgamated.hpp"
#include "../src/utils/parseUtils.hpp"

TEST_CASE("Parse unsigned hexadecimal integers")
{
    std::string hexString = "0x3";
    int expected = 0x3;
    REQUIRE(ParseUtils::parseInt(hexString, 32) == expected);

    hexString = "0x25";
    expected = 0x25;
    REQUIRE(ParseUtils::parseInt(hexString, 32) == expected);

    hexString = "0X8F3";
    expected = 0X8F3;
    REQUIRE(ParseUtils::parseInt(hexString, 32) == expected);

    hexString = "0X8F3";
    expected = 0X8F3;
    REQUIRE(ParseUtils::parseInt(hexString, 32) == expected);

    hexString = "0Xabcdef";
    expected = 0Xabcdef;
    REQUIRE(ParseUtils::parseInt(hexString, 32) == expected);

    hexString = "0x3f19ad6";
    expected = 0x3f19ad6;
    REQUIRE(ParseUtils::parseInt(hexString, 32) == expected);
}

TEST_CASE("Parse signed hexadecimal integers")
{
    std::string hexString = "0xff";
    int expected = (int)(char)0xff;
    REQUIRE(ParseUtils::parseInt(hexString, 8, true) == expected);

    hexString = "0x80";
    expected = (int)(char)0x80;
    REQUIRE(ParseUtils::parseInt(hexString, 8, true) == expected);
}
