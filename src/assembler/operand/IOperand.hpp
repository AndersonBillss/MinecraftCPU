#pragma once
#include <string>

class IOperand
{
public:
    virtual bool check(const std::string & operandText);
    virtual std::string toBin( const std::string & operandText);
};