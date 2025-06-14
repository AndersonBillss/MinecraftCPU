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

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
        if (!file.is_open())
    {
        std::cerr << "Failed to open file";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    std::string sourceCode = readFile("examples/strPtrTest.mcasm");

    std::cout << Assembler::compile(sourceCode) << std::endl;

    return 0;
}