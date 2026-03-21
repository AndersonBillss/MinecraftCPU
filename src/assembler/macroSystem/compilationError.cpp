#include "compilationError.hpp"
#include <iostream>
#include <fstream>
#include <string>

void CompilationError::printErr(std::string filePath)
{
    std::cerr << "In file " << filePath << "\n";
    std::cerr << this->what() << "\n\n";
    std::ifstream file(filePath);

    size_t lineCount = 0;
    std::string line;
    while (std::getline(file, line))
    {
        if (
            lineCount == this->begin.line - 1 || lineCount == this->begin.line - 2 ||
            lineCount == this->end.line + 1 || lineCount == this->end.line + 2)
        {
            std::cerr << lineCount + 1 << " |  ";
            std::cerr << line << "\n";
        }
        else if (lineCount >= this->begin.line && lineCount <= this->end.line)
        {
            std::cerr << lineCount + 1 << " |  ";
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

            std::cerr << "  |  ";
            for (size_t i = 0; i < line.size(); i++)
            {
                if (i >= errStart && i <= errEnd)
                {
                    std::cerr << "^";
                }
                else
                {
                    std::cerr << " ";
                }
            }
            std::cerr << "\n";
        }

        lineCount++;
    }
}
