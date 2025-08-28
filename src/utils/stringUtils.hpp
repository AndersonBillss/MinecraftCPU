#pragma once
#include <string>
#include <vector>
#include <set>

namespace stringUtils
{
    std::string trim(const std::string &text);
    bool subSectionEqual(const std::string &text, size_t startingIndex, const std::string &subSection);
    std::vector<std::string> split(const std::string &text);
    std::vector<std::string> split(const std::string &text, const std::string &delimiter);
    std::string join(const std::vector<std::string> &text, const std::string &delimiter);
    std::string getBlock(const std::string &text, const std::string &opening, const std::string &closing, int index = 0);
    size_t getOccurrences(const std::string &text, const std::string &section);
    int indexOfFirst(const std::string &text, const std::string &section);
    std::string parseEsc(const std::string &text);
}