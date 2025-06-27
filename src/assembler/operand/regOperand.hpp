#pragma once
#include "iOperand.hpp"

class RegOperand : public IOperand
{
public:
    std::string toBin(const std::string &operandText) override;

    inline int add(int a, int b)
    {
        return a + b;
    }
};