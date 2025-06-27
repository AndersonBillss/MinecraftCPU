#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "assembler/assembler.hpp"
#include "utils/stringUtils.hpp"
#include "utils/binUtils.hpp"
#include "utils/fileUtils.hpp"
#include "utils/syntaxError.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "No file provided" << std::endl;
        return 1;
    }
    std::string fileName = argv[1];
    std::string sourceCode = fileUtils::readFile(fileName);

    std::string assembled;
    try
    {
        assembled = Assembler::compile(sourceCode);
    }
    catch (const SyntaxError &e)
    {
        std::cerr << "Compilation error found in '" + fileName + "'\n -> " + e.what() << std::endl;
        return 1;
    }

    std::string filePath = "sampleFile//hello/test.txt";
    std::string contents = "HELLO, WORLD!";
    try
    {
        fileUtils::writeToFile(filePath, contents);
    }
    catch (const SyntaxError &e)
    {
        std::cerr << std::string(e.what()) + std::string(":\n -> Provided file path: '") + stringUtils::trim(filePath) + "'" << std::endl;
        return 1;
    }

    return 0;
}