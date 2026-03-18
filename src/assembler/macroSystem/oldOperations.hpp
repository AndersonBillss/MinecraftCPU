#include "oldMacroSystem.hpp"
#include <unordered_map>

extern std::unordered_map<std::string, std::function<std::variant<unsigned int, std::string>(std::variant<unsigned int, std::string>, std::variant<unsigned int, std::string>)>> oldOperations;
std::variant<unsigned int, std::string> addOperation(std::variant<unsigned int, std::string> a, std::variant<unsigned int, std::string> b);