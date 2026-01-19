#include "operations.hpp"
#include "../../utils/typeError.hpp"
#include <variant>

Operand mulStrInt(std::string a, unsigned int b)
{
    std::string result = "";
    for (size_t i = 0; i < b; i++)
    {
        if (i != 0)
        {
            result += "\n";
        }
        result += a;
    }
    return result;
}
Operand mulOperation(Operand a, Operand b)
{
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b))
    {
        return std::get<int>(a) * std::get<int>(b);
    }
    else if (std::holds_alternative<int>(a) && std::holds_alternative<std::string>(b))
    {
        return mulStrInt(std::get<std::string>(b), std::get<int>(a));
    }
    else if (std::holds_alternative<std::string>(a) && std::holds_alternative<int>(b))
    {
        return mulStrInt(std::get<std::string>(a), std::get<int>(b));
    }
    throw TypeError("Cannot multiply two strings together");

    return 0;
}

Operand divOperation(Operand a, Operand b)
{
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b))
    {
        return std::get<int>(a) / std::get<int>(b);
    }
    throw TypeError("Can only divide two integers");
}

Operand addOperation(Operand a, Operand b)
{
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b))
    {
        return std::get<int>(a) + std::get<int>(b);
    }
    else if (std::holds_alternative<int>(a) && std::holds_alternative<std::string>(b))
    {
        return std::get<std::string>(b) + std::to_string(std::get<int>(a));
    }
    else if (std::holds_alternative<std::string>(a) && std::holds_alternative<int>(b))
    {
        return std::get<std::string>(a) + std::to_string(std::get<int>(b));
    }
    return std::get<std::string>(a) + std::get<std::string>(b);
}

Operand subOperation(Operand a, Operand b)
{
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b))
    {
        int result = std::get<int>(a) - std::get<int>(b);
        return result;
    }
    throw TypeError("Can only subtract two integers");
}

Operand orOperation(Operand a, Operand b)
{
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b))
    {
        return std::get<int>(a) | std::get<int>(b);
    }
    throw TypeError("Can only OR two integers together");
}

Operand andOperation(Operand a, Operand b)
{
    if (std::holds_alternative<int>(a) && std::holds_alternative<int>(b))
    {
        return std::get<int>(a) & std::get<int>(b);
    }
    throw TypeError("Can only AND two integers together");
}

std::unordered_map<AST::NodeType, std::function<Operand(Operand, Operand)>> operations = {
    {AST::NodeType::MULTIPLY, mulOperation},
    {AST::NodeType::DIVIDE, divOperation},
    {AST::NodeType::ADD, addOperation},
    {AST::NodeType::SUB, subOperation},
    {AST::NodeType::OR, orOperation},
    {AST::NodeType::AND, andOperation},
};