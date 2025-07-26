#include <string>
#include <unordered_map>
#include <vector>

using SymbolMap = std::unordered_map<std::string, int>;
using VariableMap = std::unordered_map<std::string, std::string>;

class MacroSystem
{
    MacroSystem();
    void evaluate(std::string block);
    
    int getValue(std::string symbol, size_t stackIndex = 0);
    void setValue(std::string symbol, int value, size_t stackIndex = 0);

    std::string getMacro(std::string symbol);
    void setMacro(std::string symbol, std::string value);

    int getLabel(std::string symbol);
    void setLabel(std::string symbol, int value);


private:
    SymbolMap _labels;
    std::unordered_map<std::string, std::string> _macros; 

    std::vector<SymbolMap> _values;
};