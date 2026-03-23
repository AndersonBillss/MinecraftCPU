#pragma once
#include <string>
#include <stdexcept>
#include <sstream>
#include "sourceLocation.hpp"

enum class ErrorStage
{
    Tokenization,
    Parsing,
    Semantic,
    Codegen
};

class CompilationError : public std::exception
{
private:
    ErrorStage stage;
    SourceLocation begin;
    SourceLocation end;
    std::string message;
    std::string formatted;

public:
    CompilationError(ErrorStage s, SourceLocation start, SourceLocation end, const std::string &msg)
        : stage(s), begin(start), end(end), message(msg)
    {
        formatted = msg;
    }

    const char *what() const noexcept override
    {
        return formatted.c_str();
    }

    void printErr(std::string filePath);

private:
    void _printPadding();
    size_t _getPaddingSize();
    void _printErrorIndicators(std::string line, size_t n);
    void _printLeftSidePrefix();
    void _printLeftSidePrefix(size_t n);
};