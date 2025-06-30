#include "memOperand.hpp"
#include "../../utils/syntaxError.hpp"
#include "../../utils/binStringUtils.hpp"

std::string MemOperand::toBin(const std::string &operandText)
{
    if (operandText[0] != 'M')
    {
        throw SyntaxError("Syntax Error: Memory operands must be prefixed with 'R': " + operandText);
    }

    std::string operandValue = operandText.substr(1, operandText.length() - 1);
    int value;
    try
    {
        value = std::stoi(operandValue);
    }
    catch (const std::invalid_argument &e)
    {
        throw SyntaxError("Syntax Error: Memory values cannot contain non-numeric characters: " + operandText);
    }
    catch (const std::out_of_range &e)
    {
        throw SyntaxError("Syntax Error: Operand out of 8-bit range (0-255): " + operandText);
    }

    if (value < 0 || value > 255)
    {
        throw SyntaxError("Syntax Error: Operand out of 8-bit range (0-255): " + operandText);
    }

    return binStringUtils::toBin(value, 8);
}