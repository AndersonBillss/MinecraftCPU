#include <iostream>
#include "assembler/assembler.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "utils/stringUtils.hpp"
#include "utils/binUtils.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "utils/syntaxError.hpp"

std::string readFile(const std::string &filename)
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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "No file provided" << std::endl;
        return 1;
    }
    std::string fileName = argv[1];
    std::string sourceCode = readFile(fileName);

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

    std::cout << assembled << std::endl;

    return 0;
}