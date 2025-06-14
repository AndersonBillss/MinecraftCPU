#pragma once
#include "IOperand.hpp"

class Int8Operand : public IOperand
{
public:
    std::string toBin(const std::string &operandText) override;
};