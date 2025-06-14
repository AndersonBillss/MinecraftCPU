#include "stringUtils.hpp"
#include <cctype>

std::string stringUtils::trim(const std::string &text)
{
    size_t start = 0;
    while (start < text.length() && std::isspace(text[start]))
    {
        start++;
    }

    size_t end = text.length();
    while (end > start && std::isspace(text[end - 1]))
    {
        end--;
    }

    return text.substr(start, end - start);
}

std::vector<std::string> stringUtils::split(const std::string &text)
{
    std::vector<std::string> splitString;

    std::string trimmed = trim(text);
    std::string currWord;
    for (char c : text)
    {
        if (std::isspace(c))
        {
            if (!currWord.empty())
            {
                splitString.push_back(currWord);
            }
            currWord = "";
        }
        else
        {
            currWord.push_back(c);
        }
    }
    if (!currWord.empty())
    {
        splitString.push_back(currWord);
    }

    return splitString;
}

std::string stringUtils::join(const std::vector<std::string> &text)
{
    std::string result;
    for(int i = 0; i < text.size(); i++){
        if(i != 0) result += " ";
        result += text[i];
    }
    return result;
}
