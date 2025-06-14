#include <iostream>
#include "assembler/assembler.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include "utils/stringUtils.hpp"

int main()
{
    std::ifstream file("examples/strPtrTest.mcasm");
    if (!file.is_open())
    {
        std::cerr << "Failed to open file";
    }

    std::string line;
    while(std::getline(file, line)){
        std::cout << line << std::endl;
    }
    std::cout << Assembler::compile("") << std::endl;

    return 0;
}