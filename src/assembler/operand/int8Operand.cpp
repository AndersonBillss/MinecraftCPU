#include "int8Operand.hpp"
#include "../../utils/syntaxError.hpp"
#include "../../utils/binStringUtils.hpp"

std::string Int8Operand::toBin(const std::string &operandText)
{
    int value;
    try {
        value = std::stoi(operandText);
    } catch (const std::invalid_argument& e) {
        throw SyntaxError("Syntax Error: Int8 operands cannot contain non-numeric characters: " + operandText);
    } catch (const std::out_of_range& e) {
        throw SyntaxError("Syntax Error: Operand out of 8-bit range (0-255): " + operandText);
    }

    if (value < 0 || value > 255)
    {
        throw SyntaxError("Syntax Error: Operand out of 8-bit range (0-255): " + operandText);
    }

    return binStringUtils::toBin(value, 8);
}