#pragma once
#include "IOperand.hpp"

class RegOperand : public IOperand
{
public:
    std::string toBin(const std::string &operandText) override;
};