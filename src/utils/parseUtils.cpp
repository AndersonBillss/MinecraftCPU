#include <cmath>
#include <unordered_set>
#include <unordered_map>

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

    if (text.size() <= 3)
    {
        throw ParseUtils::ParseError(
            "Hexadecimal string must have some value");
    }

    int result = 0;
    // Start at index 2: ignore starting 0x
    for (size_t i = 2; i < text.size(); i++)
    {
        int inverted = text.size() - i;
        int placeValue = pow(4, inverted);
        auto it = hexToInt.find(text[i]);
        if (it == hexToInt.end())
        {
            throw ParseUtils::ParseError(
                "Character: " +
                std::to_string(text[i]) +
                "is not a valid hexadecimal character");
        }
        int hexValue = it->second;
        result += hexValue * placeValue;
    }
    return result;
}
int parseBinary(std::string text, unsigned char bitwidth, bool isSigned)
{
    return 0;
}
int parseDecimal(std::string text, unsigned char bitwidth)
{
    return 0;
}

int ParseUtils::parseInt(std::string text, unsigned char bitWidth, bool isSigned)
{
    bool isBinOrHex = stringUtils::subSectionEqual(text, 0, "0");
    bool isHex = stringUtils::subSectionEqual(text, 1, "x") || stringUtils::subSectionEqual(text, 1, "X");
    isHex = isBinOrHex && isHex;
    bool isBin = stringUtils::subSectionEqual(text, 1, "b") || stringUtils::subSectionEqual(text, 1, "B");
    isBin = isBinOrHex && isHex;
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
        return parseDecimal(text, bitWidth);
    }
}
