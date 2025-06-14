#include "regOperand.hpp"
#include "../../utils/syntaxError.hpp"
#include "../../utils/binUtils.hpp"
#include <stdexcept>

std::string RegOperand::toBin(const std::string &operandText)
{
    if (operandText[0] != 'R')
    {
        throw SyntaxError("Syntax Error: Register operands must be prefixed with 'R': " + operandText);
    }

    std::string operandValue = operandText.substr(1, operandText.length() - 1);
    int value;
    try
    {
        value = std::stoi(operandValue);
    }
    catch (const std::invalid_argument &e)
    {
        throw SyntaxError("Syntax Error: Register values cannot contain non-numeric characters: " + operandText);
    }
    catch (const std::out_of_range &e)
    {
        throw SyntaxError("Syntax Error: Operand out of 8-bit range (0-15): " + operandText);
    }

    if (value < 0 || value > 15)
    {
        throw SyntaxError("Syntax Error: Operand out of 8-bit range (0-15): " + operandText);
    }

    return binUtils::toBin(value, 8);
}