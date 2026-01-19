#include "oldMacroSystem.hpp"
#include <unordered_map>

extern std::unordered_map<std::string, std::function<Operand(Operand, Operand)>> operations;
Operand addOperation(Operand a, Operand b);