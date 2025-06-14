#include "binUtils.hpp"
#include "cmath"
#include <string>
#include <stdexcept>

std::string binUtils::toBin(int number, int bits)
{
    if (number < 0 || number >= pow(2, bits))
    {
        throw std::logic_error(std::to_string(number) + " cannot be represented by " + std::to_string(bits) + " bits");
    }
    int greatetstBinValue = pow(2, bits - 1);
    std::string result;
    for(int i = 0; i < bits; i++)
    {
        if (number - greatetstBinValue >= 0)
        {
            result += "1";
            number -= greatetstBinValue;
        }
        else
        {
            result += "0";
        }
        greatetstBinValue /= 2;
    } 
    return result;
}