#pragma once
#include <string>
#include <iostream>
#include <cstdlib>

class TypeError : public std::exception {
    std::string message;
public:
    TypeError(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};