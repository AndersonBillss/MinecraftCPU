#include "fileUtils.hpp"
#include "stringUtils.hpp"
#include "syntaxError.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>

std::string fileUtils::readFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " + filename << std::endl;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void fileUtils::writeToFile(const std::string &outputPath, const std::string &contents)
{
    std::string trimmedOutputPath = stringUtils::trim(outputPath);
    if (trimmedOutputPath.empty())
    {
        throw SyntaxError("File paths cannot be empty");
    }
    char lastCharacter = trimmedOutputPath[trimmedOutputPath.size() - 1];
    if (lastCharacter == '/' || lastCharacter == '\\')
    {
        throw SyntaxError("File paths cannot end in a '/' or a '\\'");
    }

    std::filesystem::path filePath = outputPath;
    std::filesystem::path parentPath = filePath.parent_path();
    if (!parentPath.empty())
    {
        std::filesystem::create_directories(parentPath);
    }
    std::ofstream outFile(outputPath);
    if (!outFile.is_open())
    {
        std::cerr << "Failed to open file: " + outputPath << std::endl;
        return;
    }
    outFile << contents;
    outFile.close();
}
