#include "errorReporter.hpp"
#include <iostream>

void ErrorReporter::printErr(CompilationError &e)
{
    std::cout << "COMPILATION ERROR" << std::endl;
}