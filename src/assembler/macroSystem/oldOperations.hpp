#include "oldMacroSystem.hpp"
#include <unordered_map>

extern std::unordered_map<std::string, std::function<Operand(Operand, Operand)>> oldOperations;
Operand addOperation(Operand a, Operand b);