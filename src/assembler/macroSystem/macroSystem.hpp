#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

enum VariableType
{
    label,
    number,
    macro,
};
struct Variable
{
    VariableType type;
    std::variant<int, std::string> value;
};
using VariableMap = std::unordered_map<std::string, Variable>;

class MacroSystem
{
public:
    MacroSystem();
    std::string evaluate(const std::string &block);

    void setNumber(std::string symbol, int value);
    void setMacro(std::string symbol, std::string value);
    void setLabel(std::string symbol, int value);
    Variable getVariable(std::string symbol);

    void pushStack();
    void popStack();

private:
    void setVariableHelper(std::string symbol, Variable value, size_t stackIndex = 0);
    Variable _getVariableHelper(std::string symbol, size_t stackIndex = 0);

    std::vector<VariableMap> _variables;
    size_t _currentStack;
};