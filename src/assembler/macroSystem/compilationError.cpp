#include "compilationError.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

void printErrStage(ErrorStage stage)
{
    switch (stage)
    {
    case ErrorStage::Parsing:
        std::cerr << "Parsing";
        break;
    case ErrorStage::Codegen:
        std::cerr << "Codegen";
        break;
    case ErrorStage::Semantic:
        std::cerr << "Semantic";
        break;
    case ErrorStage::Tokenization:
        std::cerr << "Tokenization";
        break;
    }
}

void CompilationError::printErr(std::string filePath)
{
    printErrStage(this->stage);
    std::cerr << " error: " << this->what() << "\n";
    std::ifstream file(filePath);

    size_t lineCount = 0;
    std::string line;
    std::cerr << "  ";
    printPadding();
    std::cerr << "--> "
              << filePath << ":"
              << this->begin.line + 1 << ":"
              << this->begin.column + 1
              << "\n";

    printLeftSidePrefix();
    std::cerr << "\n";
    while (std::getline(file, line))
    {
        if (
            lineCount == this->begin.line - 1 || lineCount == this->begin.line - 2 ||
            lineCount == this->end.line + 1 || lineCount == this->end.line + 2)
        {
            printLeftSidePrefix(lineCount);
            std::cerr << line << "\n";
        }
        else if (lineCount >= this->begin.line && lineCount <= this->end.line)
        {
            printLeftSidePrefix(lineCount);
            std::cerr << line << std::endl;
            printErrorIndicators(line, lineCount);
        }

        lineCount++;
    }
}

void CompilationError::printErrorIndicators(std::string line, size_t n)
{
    size_t errStart = this->begin.column;
    size_t errEnd = this->end.column;
    if (n > this->begin.line)
    {
        errStart = 0;
    }
    if (n < this->end.line)
    {
        errEnd = 0;
    }

    printLeftSidePrefix();
    bool printedCaret = false;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (i > errEnd)
        {
            break;
        }
        else if (i >= errStart)
        {
            if (!printedCaret)
            {
                std::cerr << "^";
                printedCaret = true;
            }
            else
            {
                std::cerr << "~";
            }
        }
        else
        {
            std::cerr << " ";
        }
    }
    std::cerr << "\n";
}

int countDigits(size_t num)
{
    if (num == 0)
        return 1;
    return static_cast<int>(std::log10(num)) + 1;
}

size_t CompilationError::getPaddingSize()
{
    size_t maxLineNumber = this->end.line + 3;
    size_t maxLineNumberDigits = countDigits(maxLineNumber);
    return maxLineNumberDigits;
}

void CompilationError::printPadding()
{
    size_t paddingSize = getPaddingSize();
    for (size_t i = 0; i < paddingSize; i++)
    {
        std::cerr << " ";
    }
}

void CompilationError::printLeftSidePrefix()
{
    std::cerr << "  ";
    printPadding();
    std::cerr << " |  ";
}
void CompilationError::printLeftSidePrefix(size_t n)
{
    std::cerr << "  ";
    n++;
    size_t padding = getPaddingSize() - countDigits(n);
    for (size_t i = 0; i < padding; i++)
    {
        std::cerr << " ";
    }
    std::cerr << n;
    std::cerr << " |  ";
}
