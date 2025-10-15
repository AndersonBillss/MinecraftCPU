#include "parseUtils.hpp"
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
    return false;
}

int ParseUtils::parseInt(std::string text, bool isSigned)
{
    return 0;
}
