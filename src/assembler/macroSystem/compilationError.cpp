#include "compilationError.hpp"
#include <iostream>
#include <fstream>
#include <string>

void CompilationError::printErr(std::string filePath)
{
    std::cerr << this->what() << std::endl;
    std::ifstream file(filePath);

    size_t lineCount = 1;
    std::string line;
    while (std::getline(file, line))
    {
        if (
            lineCount == this->begin.line - 1 || lineCount == this->begin.line - 2 ||
            lineCount == this->end.line + 1 || lineCount == this->end.line + 2)
        {
            std::cerr << line << "\n";
        }
        if (lineCount >= this->begin.line && lineCount <= this->end.line)
        {
            std::cerr << line << std::endl;
            size_t errStart = this->begin.column;
            size_t errEnd = this->end.column;
            if (lineCount > this->begin.line)
            {
                errStart = 0;
            }
            if (lineCount < this->end.line)
            {
                errEnd = 0;
            }

            for (size_t i = 0; i < line.size(); i++)
            {
                if (i >= errStart && i < errEnd)
                {
                    std::cout << "^" << std::endl;
                }
                else
                {
                    std::cout << " " << std::endl;
                }
            }
        }

        lineCount++;
    }
}
