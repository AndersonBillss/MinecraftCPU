#include "operations.hpp"
#include "../../utils/typeError.hpp"
#include <variant>

Operand mulSrtInt(std::string a, unsigned int b)
{
    std::string result = "";
    for (size_t i = 0; i < b; i++)
    {
        result += a + "/n";
    }
    return result;
}
Operand mulOperation(Operand a, Operand b)
{
    if (std::holds_alternative<unsigned int>(a) && std::holds_alternative<unsigned int>(b))
    {
        return std::get<unsigned int>(a) * std::get<unsigned int>(b);
    }
    else if (std::holds_alternative<unsigned int>(a) && std::holds_alternative<std::string>(b))
    {
        return mulSrtInt(std::get<std::string>(b), std::get<unsigned int>(a));
    }
    else if (std::holds_alternative<std::string>(a) && std::holds_alternative<unsigned int>(b))
    {
        return mulSrtInt(std::get<std::string>(a), std::get<unsigned int>(b));
    }
    throw TypeError("Cannot multiply two strings together");

    return (unsigned int)0;
}

Operand divOperation(Operand a, Operand b) {
    if (std::holds_alternative<unsigned int>(a) && std::holds_alternative<unsigned int>(b))
    {
        return std::get<unsigned int>(a) / std::get<unsigned int>(b);
    }
    throw TypeError("Can only divide two integers");
}

Operand addOperation(Operand a, Operand b)
{
    if (std::holds_alternative<unsigned int>(a) && std::holds_alternative<unsigned int>(b))
    {
        return std::get<unsigned int>(a) + std::get<unsigned int>(b);
    }
    else if (std::holds_alternative<unsigned int>(a) && std::holds_alternative<std::string>(b))
    {
        return std::get<std::string>(b) + std::to_string(std::get<unsigned int>(a));
    }
    else if (std::holds_alternative<std::string>(a) && std::holds_alternative<unsigned int>(b))
    {
        return std::get<std::string>(a) + std::to_string(std::get<unsigned int>(b));
    }
    return std::get<std::string>(b) + std::get<std::string>(a);
}

Operand subOperation(Operand a, Operand b)
{
    if (std::holds_alternative<unsigned int>(a) && std::holds_alternative<unsigned int>(b))
    {
        int result = std::get<unsigned int>(a) - std::get<unsigned int>(b);
        while (result < 0)
        {
            result = 256 + result;
        }
        return (unsigned int)result;
    }
    throw TypeError("Can only subtract two integers");
}

Operand orOperation(Operand a, Operand b)
{
    if (std::holds_alternative<unsigned int>(a) && std::holds_alternative<unsigned int>(b))
    {
        return std::get<unsigned int>(a) | std::get<unsigned int>(b);
    }
    throw TypeError("Can only OR two integers together");
}

Operand andOperation(Operand a, Operand b)
{
    if (std::holds_alternative<unsigned int>(a) && std::holds_alternative<unsigned int>(b))
    {
        return std::get<unsigned int>(a) & std::get<unsigned int>(b);
    }
    throw TypeError("Can only AND two integers together");
}

std::unordered_map<std::string, std::function<Operand(Operand, Operand)>> operations = {
    {"*", mulOperation},
    {"/", divOperation},
    {"+", addOperation},
    {"-", subOperation},
    {"|", orOperation},
    {"&", andOperation},
};