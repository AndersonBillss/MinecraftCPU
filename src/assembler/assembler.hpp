#pragma once
#include <string>

namespace Assembler
{
    std::string compile(const std::string &sourceCode);
    std::string expand(const std::string &sourceCode);
};