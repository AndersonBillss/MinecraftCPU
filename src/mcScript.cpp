#include <iostream>
#include "assembler/assembler.hpp"

int main()
{
    std::cout << Assembler::compile("") << std::endl;
    return 0;
}