#pragma once
#include <string>
#include <vector>
#include <set>

namespace stringUtils
{
    std::string trim(const std::string &text);
    std::vector<std::string> split(const std::string &text);
    std::vector<std::string> split(const std::string &text, const std::string &delimiter);
    std::string join(const std::vector<std::string> &text, const std::string &delimiter);
}