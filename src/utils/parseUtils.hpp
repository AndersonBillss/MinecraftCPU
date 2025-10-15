#pragma once
#include <string>
#include <cstdlib>

namespace ParseUtils
{
    bool isNumeric(std::string text);
    int parseInt(std::string text, unsigned char bitWidth, bool isSigned = false);

    class ParseError : public std::exception
    {
        std::string message;

    public:
        ParseError(const std::string &msg) : message(msg) {}
        const char *what() const noexcept override { return message.c_str(); }
    };

}
