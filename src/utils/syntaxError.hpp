#pragma once
#include <string>
#include <iostream>
#include <cstdlib>

class SyntaxError : public std::exception {
    std::string message;
public:
    SyntaxError(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};