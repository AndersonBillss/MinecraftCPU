#pragma once
#include <cctype>
#include <string>

struct SourceLocation
{
    size_t line;
    size_t column;

    friend bool operator==(const SourceLocation a, const SourceLocation b)
    {
        return a.line == b.line && a.column == b.column;
    }

    std::string stringify() const
    {
        return "(" + std::to_string(this->line) + "," + std::to_string(this->column) + ")";
    }
    friend std::ostream &operator<<(std::ostream &os,
                                    const SourceLocation &loc)
    {
        return os << loc.stringify();
    }
};
