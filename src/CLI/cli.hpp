#pragma once
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "cliError.hpp"
#include "../utils/stringUtils.hpp"

namespace Cli
{
    class Options
    {
    private:
        using optionValue = std::variant<std::string, bool>;
        struct Option
        {
            bool hasDefault;
            optionValue defaultValue;
            bool hasImplicit;
            optionValue implicitValue;
            std::string shortFlag;
            std::string longFlag;
            std::string helpText;
        };
        class StringOption
        {
        private:
            Option *_data;

        public:
            StringOption(Option *data)
            {
                _data = data;
            }
            StringOption &addDefault(const std::string &value)
            {
                _data->hasDefault = true;
                _data->defaultValue = value;
                return *this;
            }
            StringOption &addImplicit(const std::string &value)
            {
                _data->hasImplicit = true;
                _data->implicitValue = value;
                return *this;
            }
            StringOption &addHelp(const std::string &help)
            {
                _data->helpText = help;
                return *this;
            }
        };
        class BoolOption
        {
        private:
            Option *_data;

        public:
            BoolOption(Option *data)
            {
                _data = data;
            }
            BoolOption &addHelp(std::string help)
            {
                _data->helpText = help;
                return *this;
            }
        };

        std::map<std::string, std::unique_ptr<Option>> _longOptions;
        std::map<std::string, Option *> _shortOptions;

    public:
        StringOption stringOption(const std::string &longFlag, const std::string &shortFlag = "")
        {
            std::string trimmedLongFlag = stringUtils::trim(longFlag);
            std::string trimmedShortFlag = stringUtils::trim(shortFlag);
            if (trimmedLongFlag.empty())
            {
                throw CliError("Long flag must be provided");
            }
            auto option = std::make_unique<Option>();
            option->hasDefault = false;
            option->defaultValue = "";
            option->hasImplicit = false;
            option->implicitValue = "";
            option->shortFlag = trimmedShortFlag;
            option->longFlag = trimmedLongFlag;
            option->helpText = "";
            Option *raw = option.get();
            _longOptions[trimmedLongFlag] = std::move(option);
            if (!trimmedShortFlag.empty())
            {
                _shortOptions[trimmedShortFlag] = raw;
            }
            return StringOption(raw);
        }

        BoolOption boolOption(const std::string &longFlag, const std::string &shortFlag = "")
        {
            std::string trimmedLongFlag = stringUtils::trim(longFlag);
            std::string trimmedShortFlag = stringUtils::trim(shortFlag);
            if (trimmedLongFlag.empty())
            {
                throw CliError("Long flag must be provided");
            }
            auto option = std::make_unique<Option>();
            option->hasDefault = false;
            option->defaultValue = false;
            option->hasImplicit = false;
            option->implicitValue = false;
            option->shortFlag = trimmedShortFlag;
            option->longFlag = trimmedLongFlag;
            option->helpText = "";

            Option *raw = option.get();
            _longOptions[trimmedLongFlag] = std::move(option);
            if (!trimmedShortFlag.empty())
            {
                _shortOptions[trimmedShortFlag] = raw;
            }
            return BoolOption(raw);
        }
    };
}
