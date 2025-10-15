#include "parseUtils.hpp"
#include "stringUtils.hpp"
#include <unordered_set>

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

int parseHexadecimal(std::string text, bool isSigned)
{
    return 0;
}
int parseBinary(std::string text, bool isSigned)
{
    return 0;
}
int parseDecimal(std::string text)
{
    return 0;
}

int ParseUtils::parseInt(std::string text, bool isSigned)
{
    bool isBinOrHex = stringUtils::subSectionEqual(text, 0, "0");
    bool isHex = stringUtils::subSectionEqual(text, 1, "x") || stringUtils::subSectionEqual(text, 1, "X");
    isHex = isBinOrHex && isHex;
    bool isBin = stringUtils::subSectionEqual(text, 1, "b") || stringUtils::subSectionEqual(text, 1, "B");
    isBin = isBinOrHex && isHex;
    if (isHex)
    {
        return parseHexadecimal(text, isSigned);
    }
    else if (isBin)
    {
        return parseBinary(text, isSigned);
    }
    else
    {
        return parseDecimal(text);
    }
}
