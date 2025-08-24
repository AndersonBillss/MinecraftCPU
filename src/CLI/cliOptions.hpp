#pragma once
#include <variant>
#include <string>
#include <vector>
#include <map>

#include "cliError.hpp"
#include "../utils/stringUtils.hpp"

class CliOptions
{
public:
private:
    template <typename T,
              typename = std::enable_if_t<
                  std::is_same_v<T, bool> || std::is_same_v<T, std::string>>>
    class Option
    {
    private:
        bool _hasDefault;
        T _defaultValue;
        bool _hasImplicit;
        T _implicitValue;
        std::string _helpString;
        std::string _shortFlag;
        std::string _longFlag;

    public:
        Option(const std::string &longFlag, const std::string &shortFlag = "")
        {
            _valueType = type;
            if (stringUtils::trim(longFlag) == "")
            {
                throw CliError("Long flag must be provided");
            }
        }

        Option<T> addDefault(T value)
        {
            _hasDefault = true;
            _defaultValue = value;
        };
        Option<T> addImplicit(T value)
        {
            _hasImplicit = true;
            _implicitValue = value;
        };
        Option<T> addHelp(std::string help)
        {
            help = help
        }
    };
    std::vector<Option<bool>> _boolOptions;
    std::vector<Option<std::string>> _stringOptions;
};