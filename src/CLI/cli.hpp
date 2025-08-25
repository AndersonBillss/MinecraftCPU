#pragma once
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <memory>

#include "cliError.hpp"
#include "../utils/stringUtils.hpp"

namespace Cli
{
    using optionValue = std::variant<std::string, bool>;

    class Parsed
    {
    private:
        struct ParsedOption
        {
            size_t count;
            optionValue data;
        };
        std::unordered_map<std::string, optionValue> _parsedOptions;

    public:
        optionValue operator[](const std::string &key);
        size_t count(const std::string &key);

        void ensureExclusive(const std::vector<std::string> &keys);
        void ensureUsedOnce();
        void ensureUsedOnce(const std::vector<std::string> &keys);
    };

    class Options
    {
    private:
        void _handleFlagArgument(
            std::vector<std::string> &tokens,
            size_t &index,
            std::unordered_map<std::string, Cli::optionValue> &parsedMap);

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

        friend class Parsed;

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
            option->shortFlag = "-" + trimmedShortFlag;
            option->longFlag = "--" + trimmedLongFlag;
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
            option->shortFlag = "-" + trimmedShortFlag;
            option->longFlag = "--" + trimmedLongFlag;
            option->helpText = "";
            Option *raw = option.get();
            _longOptions[trimmedLongFlag] = std::move(option);
            if (!trimmedShortFlag.empty())
            {
                _shortOptions[trimmedShortFlag] = raw;
            }
            return BoolOption(raw);
        }

        void printHelp();
        Parsed parse(int argc, char *argv[]);
    };
}
