#include <cmath>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

#include "parseUtils.hpp"
#include "stringUtils.hpp"

bool ParseUtils::isNumeric(std::string text)
{
    std::unordered_set<char> numericChars = {
        '-',
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
    };
    auto it = numericChars.find(text[0]);
    return it != numericChars.end();
}

int parseHexadecimal(std::string text, unsigned char bitwidth, bool isSigned)
{
    std::unordered_map<char, int> hexToInt = {
        {'0', 0x0},
        {'1', 0x1},
        {'2', 0x2},
        {'3', 0x3},
        {'4', 0x4},
        {'5', 0x5},
        {'6', 0x6},
        {'7', 0x7},
        {'8', 0x8},
        {'9', 0x9},
        {'a', 0xa},
        {'b', 0xb},
        {'c', 0xc},
        {'d', 0xd},
        {'e', 0xe},
        {'f', 0xf},
        {'A', 0xa},
        {'B', 0xb},
        {'C', 0xc},
        {'D', 0xd},
        {'E', 0xe},
        {'F', 0xf},
    };

    if (text.size() <= 2)
    {
        throw ParseUtils::ParseError(
            "Hexadecimal string must have some value");
    }

    int result = 0;
    // Start at index 2: ignore starting 0x
    for (size_t i = 2; i < text.size(); i++)
    {
        int inverted = text.size() - i - 1;
        int placeValue = pow(16, inverted);
        auto it = hexToInt.find(text[i]);
        if (it == hexToInt.end())
        {
            throw ParseUtils::ParseError(
                std::string("Character '") +
                text[i] +
                "' is not a valid hexadecimal character");
        }
        int hexValue = it->second;
        result += hexValue * placeValue;
    }
    int largestBit = pow(2, bitwidth - 1);
    if (isSigned && result >= largestBit)
    {
        result -= largestBit * 2;
    }
    return result;
}
int parseBinary(std::string text, unsigned char bitwidth, bool isSigned)
{
    std::unordered_map<char, int> binToInt = {
        {'0', 0b0},
        {'1', 0b1},
    };

    if (text.size() <= 2)
    {
        throw ParseUtils::ParseError(
            "Binary string must have some value");
    }

    int result = 0;
    // Start at index 2: ignore starting 0b
    for (size_t i = 2; i < text.size(); i++)
    {
        int inverted = text.size() - i - 1;
        int placeValue = pow(2, inverted);
        auto it = binToInt.find(text[i]);
        if (it == binToInt.end())
        {
            throw ParseUtils::ParseError(
                std::string("Character '") +
                text[i] +
                "' is not a valid binary character");
        }
        int hexValue = it->second;
        result += hexValue * placeValue;
    }
    int largestBit = pow(2, bitwidth - 1);
    if (isSigned && result >= largestBit)
    {
        result -= largestBit * 2;
    }
    return result;
}
int parseDecimal(std::string text, unsigned char bitwidth, bool isSigned)
{
    bool isNegative = false;
    size_t startingIndex = 0;
    if (text[0] == '-')
    {
        isNegative = true;
        startingIndex = 1;
    }
    std::unordered_map<char, int> hexToInt = {
        {'0', 0},
        {'1', 1},
        {'2', 2},
        {'3', 3},
        {'4', 4},
        {'5', 5},
        {'6', 6},
        {'7', 7},
        {'8', 8},
        {'9', 9},
    };

    if (text.size() - startingIndex < 0)
    {
        throw ParseUtils::ParseError(
            "Integer string must have some value");
    }

    int result = 0;
    for (size_t i = startingIndex; i < text.size(); i++)
    {
        int inverted = text.size() - i - 1;
        int placeValue = pow(10, inverted);
        auto it = hexToInt.find(text[i]);
        if (it == hexToInt.end())
        {
            throw ParseUtils::ParseError(
                std::string("Character '") +
                text[i] +
                "' is not a valid hexadecimal character");
        }
        int hexValue = it->second;
        result += hexValue * placeValue;
    }
    if (isNegative)
    {
        result = 0 - result;
    }
    int largestBit = pow(2, bitwidth - 1);
    if (isSigned && result >= largestBit)
    {
        result -= largestBit * 2;
    }
    return result;
}

int ParseUtils::parseInt(std::string text, unsigned char bitWidth, bool isSigned)
{
    bool isBinOrHex = stringUtils::subSectionEqual(text, 0, "0");
    bool isHex = stringUtils::subSectionEqual(text, 1, "x") || stringUtils::subSectionEqual(text, 1, "X");
    isHex = isBinOrHex && isHex;
    bool isBin = stringUtils::subSectionEqual(text, 1, "b") || stringUtils::subSectionEqual(text, 1, "B");
    isBin = isBinOrHex && isBin;
    if (isHex)
    {
        return parseHexadecimal(text, bitWidth, isSigned);
    }
    else if (isBin)
    {
        return parseBinary(text, bitWidth, isSigned);
    }
    else
    {
        return parseDecimal(text, bitWidth, isSigned);
    }
}
