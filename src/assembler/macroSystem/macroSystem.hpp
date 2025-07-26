#include <string>
#include <unordered_map>
#include <stack>

using SymbolMap = std::unordered_map<std::string, int>;
using VariableMap = std::unordered_map<std::string, std::string>;

class MacroSystem
{
    MacroSystem();
    void evaluateBlock(std::string block);
    
    void getValue(std::string symbol);
    void setValue(std::string symbol, int value);

    void getMacro(std::string symbol);
    void setValue(std::string symbol, std::string value);

    void getLabel(std::string symbol);
    void setLabel(std::string symbol, int value);


private:
    SymbolMap _labels;
    std::unordered_map<std::string, std::string> _macros; 
    std::stack<SymbolMap> _values;
};