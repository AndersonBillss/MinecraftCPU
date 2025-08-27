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
        std::unordered_map<std::string, ParsedOption> _parsedOptions;

    public:
        template <typename T>
        T get(const std::string &key)
        {
            auto it = _parsedOptions.find(key);
            if (it == _parsedOptions.end())
            {
                throw CliError("Cannot access flag: '" + key + "'");
            }
            return std::get<T>(it->second.data);
        };
        size_t count(const std::string &key);

        void ensureExclusive(const std::vector<std::string> &keys);

        friend class Options;
    };

    class Options
    {
    private:
        std::string _title;
        std::string _description;

        void _handleFlagArgument(
            std::vector<std::string> &tokens,
            size_t &index,
            std::unordered_map<std::string, Parsed::ParsedOption> &parsedMap);

        struct Option
        {
            bool hasDefault;
            optionValue defaultValue;
            bool hasImplicit;
            optionValue implicitValue;
            bool multipleAllowed;
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
            StringOption &allowMultiple()
            {
                _data->multipleAllowed = true;
                return *this;
            };
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
            BoolOption &allowMultiple()
            {
                _data->multipleAllowed = true;
                return *this;
            };
        };

        std::map<std::string, std::unique_ptr<Option>> _longOptions;
        std::map<std::string, Option *> _shortOptions;

    public:
        Options(const std::string &title, const std::string &description)
        {
            _title = title;
            _description = description;
        }
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
            _longOptions["--" + trimmedLongFlag] = std::move(option);
            if (!trimmedShortFlag.empty())
            {
                _shortOptions["-" + trimmedShortFlag] = raw;
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
            _longOptions["--" + trimmedLongFlag] = std::move(option);
            if (!trimmedShortFlag.empty())
            {
                _shortOptions["-" + trimmedShortFlag] = raw;
            }
            return BoolOption(raw);
        }

        void printHelp();
        Parsed parse(int argc, char *argv[]);
    };
};
