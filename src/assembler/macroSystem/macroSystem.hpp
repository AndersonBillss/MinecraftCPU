#include <string>
#include <unordered_map>
#include <stack>

using InstructionLocationMap = std::unordered_map<std::string, int>;
using VariableMap = std::unordered_map<std::string, std::string>;

class MacroSystem
{
    MacroSystem();
    void evaluateBlock(std::string block);
    void getValue(std::string symbol);
    void setValue(std::string symbol, std::string value);

private:
    std::stack<VariableMap> _variables;
    InstructionLocationMap _instructions;
};