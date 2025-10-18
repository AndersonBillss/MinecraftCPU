#include <string>

#include "catch_amalgamated.hpp"
#include "../src/utils/parseUtils.hpp"

TEST_CASE("Parse hexadecimal integers")
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

    hexString = "0xa9";
    expected = (int)(char)0xa9;
    REQUIRE(ParseUtils::parseInt(hexString, 8, true) == expected);
}

TEST_CASE("Parse unsigned hexadecimal integers")
{
    std::string hexString = "0xff";
    int expected = (int)(unsigned char)0xff;
    REQUIRE(ParseUtils::parseInt(hexString, 8, false) == expected);

    hexString = "0x80";
    expected = (int)(unsigned char)0x80;
    REQUIRE(ParseUtils::parseInt(hexString, 8, false) == expected);

    hexString = "0xa9";
    expected = (int)(unsigned char)0xa9;
    REQUIRE(ParseUtils::parseInt(hexString, 8, false) == expected);
}

TEST_CASE("Parse signed binary integers")
{
    std::string binString = "0b11111111";
    int expected = (int)(char)0b11111111;
    REQUIRE(ParseUtils::parseInt(binString, 8, true) == expected);

    binString = "0b10000000";
    expected = (int)(char)0b10000000;
    REQUIRE(ParseUtils::parseInt(binString, 8, true) == expected);

    binString = "0b10110101";
    expected = (int)(char)0b10110101;
    REQUIRE(ParseUtils::parseInt(binString, 8, true) == expected);
}

TEST_CASE("Parse unsigned binary integers")
{
    std::string binString = "0b11111111";
    int expected = (int)(unsigned char)0b11111111;
    REQUIRE(ParseUtils::parseInt(binString, 8, false) == expected);

    binString = "0b10000000";
    expected = (int)(unsigned char)0b10000000;
    REQUIRE(ParseUtils::parseInt(binString, 8, false) == expected);

    binString = "0b10110101";
    expected = (int)(unsigned char)0b10110101;
    REQUIRE(ParseUtils::parseInt(binString, 8, false) == expected);
}

TEST_CASE("Parse signed decimal integers")
{
    std::string decimalString = "-1";
    int expected = (int)(char)-1;
    REQUIRE(ParseUtils::parseInt(decimalString, 8, true) == expected);

    decimalString = "255";
    expected = (int)(char)255;
    REQUIRE(ParseUtils::parseInt(decimalString, 8, true) == expected);

    decimalString = "128";
    expected = (int)(char)128;
    REQUIRE(ParseUtils::parseInt(decimalString, 8, true) == expected);

    decimalString = "199";
    expected = (int)(char)199;
    REQUIRE(ParseUtils::parseInt(decimalString, 8, true) == expected);
}

TEST_CASE("Parse unsigned decimal integers")
{
    std::string decimalString = "-1";
    int expected = (int)(unsigned char)-1;
    REQUIRE(ParseUtils::parseInt(decimalString, 8, false) == expected);

    decimalString = "255";
    expected = (int)(unsigned char)255;
    REQUIRE(ParseUtils::parseInt(decimalString, 8, false) == expected);

    decimalString = "128";
    expected = (int)(unsigned char)128;
    REQUIRE(ParseUtils::parseInt(decimalString, 8, false) == expected);

    decimalString = "199";
    expected = (int)(unsigned char)199;
    REQUIRE(ParseUtils::parseInt(decimalString, 8, false) == expected);
}