#include "catch_amalgamated.hpp"
#include "../src/utils/stringUtils.hpp"
#include <string>
#include <vector>

TEST_CASE("StringUtils join")
{
    std::vector<std::string> testVec;
    std::string expected;
    std::string delimiter;

    testVec = {"1", "2", "3", "4", "5", "6"};
    delimiter = "";
    expected = "123456";
    REQUIRE(stringUtils::join(testVec, delimiter) == expected);

    testVec = {"This", "is", "a", "sample", "string"};
    delimiter = " ";
    expected = "This is a sample string";
    REQUIRE(stringUtils::join(testVec, delimiter) == expected);

    testVec = {"This", "is", "a", "more", "complex", "test", "case"};
    delimiter = "1234";
    expected = "This1234is1234a1234more1234complex1234test1234case";
    REQUIRE(stringUtils::join(testVec, delimiter) == expected);
}

TEST_CASE("StringUtils trim")
{
    std::string testString;
    std::string expected;

    testString = "0";
    expected = "0";
    REQUIRE(stringUtils::trim(testString) == expected);

    testString = "12";
    expected = "12";
    REQUIRE(stringUtils::trim(testString) == expected);

    testString = " 1 ";
    expected = "1";
    REQUIRE(stringUtils::trim(testString) == expected);

    testString = " 1 2 3 ";
    expected = "1 2 3";
    REQUIRE(stringUtils::trim(testString) == expected);

    testString = "   2   ";
    expected = "2";
    REQUIRE(stringUtils::trim(testString) == expected);

    testString = " \n \t 3  \n\t\n\n\n   \t\t ";
    expected = "3";
    REQUIRE(stringUtils::trim(testString) == expected);

    testString = "\n\n This is \n a sample \t\t\t string \n\n\n";
    expected = "This is \n a sample \t\t\t string";
    REQUIRE(stringUtils::trim(testString) == expected);
}

TEST_CASE("StringUtils Split (white space)")
{
    std::string testString;
    std::vector<std::string> expected;

    testString = "1 2 3 4 5 6";
    expected = {"1", "2", "3", "4", "5", "6"};
    REQUIRE(stringUtils::split(testString) == expected);

    testString = "  1 2 3 4 5 6  ";
    expected = {"1", "2", "3", "4", "5", "6"};
    REQUIRE(stringUtils::split(testString) == expected);

    testString = " This   is a    test \t\n string     1 \n";
    expected = {"This", "is", "a", "test", "string", "1"};
    REQUIRE(stringUtils::split(testString) == expected);
}

TEST_CASE("StringUtils Split (delimiter)")
{
    std::string testString;
    std::string delimiter;
    std::vector<std::string> expected;

    testString = "1a2a3a4a5a6";
    delimiter = "a";
    expected = {"1", "2", "3", "4", "5", "6"};
    REQUIRE(stringUtils::split(testString, delimiter) == expected);

    testString = "2ab3ab4ab5ab6ab7";
    delimiter = "ab";
    expected = {"2", "3", "4", "5", "6", "7"};
    REQUIRE(stringUtils::split(testString, delimiter) == expected);

    testString = "cd3cd4cd5cd6cd7cd8";
    delimiter = "cd";
    expected = {"3", "4", "5", "6", "7", "8"};
    REQUIRE(stringUtils::split(testString, delimiter) == expected);

    testString = "4de5de6de7de8de9de";
    delimiter = "de";
    expected = {"4", "5", "6", "7", "8", "9"};
    REQUIRE(stringUtils::split(testString, delimiter) == expected);

    testString = "test/file/path/to/output.txt";
    delimiter = "/";
    expected = {"test", "file", "path", "to", "output.txt"};
    REQUIRE(stringUtils::split(testString, delimiter) == expected);

    testString = "123This123123is123a123123sample123123123string123123123";
    delimiter = "123";
    expected = {"This", "is", "a", "sample", "string"};
    REQUIRE(stringUtils::split(testString, delimiter) == expected);

    testString = "123This123123is123a123123sample123123123string12312312";
    delimiter = "123";
    expected = {"This", "is", "a", "sample", "string", "12"};
    REQUIRE(stringUtils::split(testString, delimiter) == expected);
}

TEST_CASE("StringUtils getBlock")
{
    std::string testString = "Hello, (this is a test string) in parentheses";
    std::string expected = "(this is a test string)";
    REQUIRE(stringUtils::getBlock(testString, "(", ")") == expected);

    testString = "Hello, (this is a test string (with nested blocks)) in parentheses";
    expected = "(this is a test string (with nested blocks))";
    REQUIRE(stringUtils::getBlock(testString, "(", ")") == expected);

    expected = "(with nested blocks)";
    REQUIRE(stringUtils::getBlock(testString, "(", ")", 30) == expected);

    testString = "This is a block {With nested blocks {and some more {nested} blocks inside}}";
    expected = "{and some more {nested} blocks inside}";
    REQUIRE(stringUtils::getBlock(testString, "{", "}", 36) == expected);
}

TEST_CASE("StringUtils getOccurrences")
{
    std::string testString = "Hello, this is a test string with some letters";
    std::string subSection = " ";
    size_t expected = 8;
    REQUIRE(stringUtils::getOccurrences(testString, subSection) == expected);


    testString = "ababa aba abaaba a ba ab a b";
    subSection = "aba";
    expected = 4;
    REQUIRE(stringUtils::getOccurrences(testString, subSection) == expected);
}