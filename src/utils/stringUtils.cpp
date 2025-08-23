#include "stringUtils.hpp"
#include <cctype>
#include <iostream>

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

bool subSectionEqual(const std::string &text, size_t startingIndex, const std::string &subSection)
{
    for (size_t subSectionIndex = 0; subSectionIndex < subSection.size(); subSectionIndex++)
    {
        size_t textIndex = subSectionIndex + startingIndex;
        if (text[textIndex] != subSection[subSectionIndex])
            return false;
    }
    return true;
}

std::vector<std::string> stringUtils::split(const std::string &text, const std::string &delimiter)
{
    std::vector<std::string> splitString;

    std::string currWord = "";
    for (size_t i = 0; i < text.size(); i++)
    {
        if (subSectionEqual(text, i, delimiter))
        {
            i += delimiter.size() - 1;
            if (!std::empty(currWord))
            {
                splitString.push_back(currWord);
                currWord = "";
            }
        }
        else
        {
            currWord += text[i];
        }
    }
    if (!std::empty(currWord))
    {
        splitString.push_back(currWord);
        currWord = "";
    }
    return splitString;
}

std::string stringUtils::join(const std::vector<std::string> &text, const std::string &delimiter)
{
    std::string result;
    for (size_t i = 0; i < text.size(); i++)
    {
        if (i != 0)
            result += delimiter;
        result += text[i];
    }
    return result;
}

std::string stringUtils::getBlock(const std::string &text, const std::string &opening, const std::string &closing, int index)
{
    std::string result;
    bool insideBlock = false;
    int depth = 0;
    for (size_t i = index; i < text.size(); i++)
    {
        if (insideBlock && subSectionEqual(text, i, closing))
        {
            i += opening.size() - 1;
            depth--;
        }
        else if (subSectionEqual(text, i, opening))
        {
            insideBlock = true;
            i += opening.size() - 1;
            depth++;
        }
        if (insideBlock)
        {
            result += text[i];
        }
        if (insideBlock && depth == 0)
        {
            return result;
        }
    }
    throw std::runtime_error("Unclosed block: missing matching closing string '" + closing + "'");
}

size_t stringUtils::getOccurrences(const std::string &text, const std::string &section)
{
    size_t index = 0;
    size_t count = 0;
    while (index < text.size())
    {
        if (subSectionEqual(text, index, section))
        {
            count++;
            index += section.size();
        }
        else
        {
            index++;
        }
    }
    return count;
}

size_t stringUtils::indexOfFirst(const std::string &text, const std::string &section)
{
    size_t foundIndex = 0;
    while (foundIndex < text.size())
    {
        if (subSectionEqual(text, foundIndex, section))
        {
            return foundIndex;
        }
        foundIndex++;
    }
    return -1;
}

void decodeEscape(const std::string &text, std::string &parsed, size_t &i)
{
    i++;
    if(i < parsed.size()){
        throw std::runtime_error("Unmatched escape at the end of the string");
    }
    char esc = text[i];
    switch (esc)
    {
    case 'n':
        parsed.push_back('\n');
        break;
    case 't':
        parsed.push_back('\t');
        break;
    case 'r':
        parsed.push_back('\r');
        break;
    case 'b':
        parsed.push_back('\b');
        break;
    case 'v':
        parsed.push_back('\v');
        break;
    case '\\':
        parsed.push_back('\\');
        break;
    case '\'':
        parsed.push_back('\'');
        break;
    case '"':
        parsed.push_back('"');
        break;
    case '?':
        parsed.push_back('?');
        break;
    default:
        throw std::runtime_error(std::string("Unknown escape: \\") + esc);
    }
}
std::string stringUtils::parseEsc(const std::string &text)
{
    std::string parsed = "";
    size_t i = 0;
    while (i < text.size())
    {
        if (text[i] == '\\')
        {
            decodeEscape(text, parsed, i);
        }
        else
        {
            parsed.push_back(text[i]);
        }
        i++;
    }
    return parsed;
}
