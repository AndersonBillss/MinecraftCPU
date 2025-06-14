#pragma once
#include "IOperand.hpp"

class Int8Operand : public IOperand
{
public:
    bool check(const std::string &operandText) override;
    std::string toBin(const std::string &operandText) override;
};