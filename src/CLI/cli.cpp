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
    args.push_back(arg.substr(equalIndex + 1, args.size() - 1));
}
std::vector<std::string> tokenize(int argc, char *argv[])
{
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
    {
        std::string argStr(args[i]);
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
    std::unordered_map<std::string, Cli::optionValue> &parsedMap)
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
    }
    else if (std::holds_alternative<bool>(option.defaultValue)) // I don't use default boolean values, but I can still check whether this value is supposed to be a bool because default value should still hold a boolean
    {
    }
}
Cli::Parsed Cli::Options::parse(int argc, char *argv[])
{
    std::vector<std::string> tokens = tokenize(argc, argv);
    std::unordered_map<std::string, Cli::optionValue> parsedMap;

    size_t i = 0;
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
    }

    return Parsed();
}
