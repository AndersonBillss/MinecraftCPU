#include "cli.hpp"

void tokenizeFlag(std::vector<std::string> &args, std::string arg)
{
    int equalIndex = stringUtils::indexOfFirst(arg, "=");
    if (equalIndex == -1)
    {
        args.push_back(arg);
        return;
    }
    args.push_back(arg.substr(0, equalIndex));
    args.push_back(arg.substr(equalIndex + 1, arg.size() - 1));
}
std::vector<std::string> tokenize(int argc, char *argv[])
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
    {
        std::string argStr(argv[i]);
        if (argStr[0] == '-')
        {
            tokenizeFlag(args, argStr);
        }
        else
        {
            args.push_back(argStr);
        }
    }
    return args;
}
void Cli::Options::_handleFlagArgument(
    std::vector<std::string> &tokens,
    size_t &index,
    std::unordered_map<std::string, Cli::Parsed::ParsedOption> &parsedMap)
{
    std::string &token = tokens[index];
    Cli::Options::Option option;
    if (token.size() > 1 && token[1] == '-')
    {
        auto it = _longOptions.find(token);
        if (it == _longOptions.end())
        {
            throw CliError("Invalid option: " + token);
        }
        option = *_longOptions[token];
    }
    else
    {
        auto it = _shortOptions.find(token);
        if (it == _shortOptions.end())
        {
            throw CliError("Invalid option: " + token);
        }
        option = *_shortOptions[token];
    }
    if (std::holds_alternative<std::string>(option.defaultValue))
    {
        Cli::Parsed::ParsedOption parsedOption = {
            count : 1,
            data : "",
        };
        auto it = parsedMap.find(option.longFlag);
        if (it != parsedMap.end())
        {
            throw CliError("Can only have one '" + option.longFlag + "' option");
            // I am keeping this code in case I want to allow duplicates.
            parsedOption = it->second;
            parsedOption.count++;
        }
        index++;
        bool hasArgument = true;
        if (index >= tokens.size())
        {
            hasArgument = false;
        }
        else if (tokens[index][0] == '-') // This means that the next token is another option
        {
            hasArgument = false;
        }

        std::string argument;
        if (hasArgument)
        {
            argument = stringUtils::parseEsc(tokens[index]);
        }
        else
        {
            if (option.hasImplicit)
            {
                argument = std::get<std::string>(option.implicitValue);
            }
            else
            {
                throw CliError("No value provided for '" + option.longFlag + "' option");
            }
        }
        parsedOption.data = argument;
        parsedMap[option.longFlag] = parsedOption;
    }
    else if (std::holds_alternative<bool>(option.defaultValue))
    // I don't use default boolean values, but I can still check whether this value
    // is supposed to be a bool because default value should still hold a boolean
    {
        Cli::Parsed::ParsedOption parsedOption = {
            count : 1,
            data : true,
        };
        auto it = parsedMap.find(option.longFlag);
        if (it != parsedMap.end())
        {
            throw CliError("Can only have one '" + option.longFlag + "' option");
            // I am keeping this code in case I want to allow duplicates.
            parsedOption = it->second;
            parsedOption.count++;
        }
        parsedMap[option.longFlag] = parsedOption;
    }
}
Cli::Parsed Cli::Options::parse(int argc, char *argv[])
{
    std::vector<std::string> tokens = tokenize(argc, argv);
    std::unordered_map<std::string, Cli::Parsed::ParsedOption> parsedMap;

    size_t i = 1;
    while (i < tokens.size())
    {
        std::string &token = tokens[i];
        if (token[0] == '-')
        {
            Cli::Options::_handleFlagArgument(tokens, i, parsedMap);
        }
        else
        {
            throw CliError("Invalid flag: " + token);
        }
        i++;
    }

    // Add default values if necessary
    for (const auto &[key, value] : _longOptions)
    {
        if (value->hasDefault)
        {
            if (parsedMap.find(value->longFlag) == parsedMap.end())
            {
                Cli::Parsed::ParsedOption newOption{
                    count : 1,
                    data : value->defaultValue
                };
                parsedMap[value->longFlag] = newOption;
            }
        }
    }

    Parsed result;
    result._parsedOptions = parsedMap;
    return result;
}

size_t Cli::Parsed::count(const std::string &key)
{
    auto it = _parsedOptions.find(key);
    if (it == _parsedOptions.end())
    {
        return 0;
    }
    return it->second.count;
}
