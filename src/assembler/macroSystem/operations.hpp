#include <unordered_map>
#include <variant>
#include <string>
#include <functional>
#include "AST.hpp"

using Operand = std::variant<int, std::string>; 
extern std::unordered_map<AST::NodeType, std::function<Operand(Operand, Operand)>> operations;