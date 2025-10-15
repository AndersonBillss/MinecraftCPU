#pragma once
#include <string>

namespace ParseUtils
{
    bool isNumeric(std::string text);
    int parseInt(std::string text, bool isSigned = false);
}