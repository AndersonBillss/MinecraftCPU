#pragma once
#include <string>

class IOperand
{
public:
    virtual std::string toBin( const std::string & operandText) = 0;
};