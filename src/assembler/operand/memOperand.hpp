#pragma once
#include "iOperand.hpp"

class MemOperand : public IOperand
{
public:
    std::string toBin(const std::string &operandText) override;
};