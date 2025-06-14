#pragma once
#include <string>

class SyntaxError {
public:
    std::string message;
    SyntaxError(const std::string& msg) : message(msg) {}
};