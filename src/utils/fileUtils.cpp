#include "fileUtils.hpp"
#include "stringUtils.hpp"
#include <iostream>
#include <sstream>
#include <vector>

// std::ofstream fileUtils::createFile(std::string outputPath)
// {
//     std::ofstream outFile("Test File");
//     return outFile;
// }

std::string fileUtils::readFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file: " + filename;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void fileUtils::writeToFile(std::string outputPath, std::string contents)
{
    std::cout << contents << std::endl;
    std::cout << outputPath << std::endl;
    std::vector<std::string> splitPath = stringUtils::split(outputPath, "/");
    for (std::string item : splitPath)
    {
        std::cout << item << std::endl; 
    }

    std::ofstream outFile(outputPath);
    std::cout << (outFile.is_open() ? "File is open" : "File is closed") << std::endl;
}
