#include <string>
#include <unordered_map>
#include <vector>

using SymbolMap = std::unordered_map<std::string, int>;
using VariableMap = std::unordered_map<std::string, std::string>;

class MacroSystem
{
public:
    MacroSystem();
    void evaluate(std::string block);
    
    int getValue(std::string symbol);
    void setValue(std::string symbol, int value);

    std::string getMacro(std::string symbol);
    void setMacro(std::string symbol, std::string value);

    int getLabel(std::string symbol);
    void setLabel(std::string symbol, int value);
    void pushStack();
    void popStack();

private:
    SymbolMap _labels;
    std::unordered_map<std::string, std::string> _macros; 
    std::vector<SymbolMap> _values;
    size_t _currentStack;


    int _getValueHelper(std::string symbol, size_t stackIndex = 0);
    void _setValueHelper(std::string symbol, int value, size_t stackIndex = 0);
};