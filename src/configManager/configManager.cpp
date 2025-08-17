#include "configManager.hpp"
#include "../utils/fileUtils.hpp"
#include "../utils/stringUtils.hpp"
#include <filesystem>

const std::string configFile = ".config";

namespace configManager
{
    std::unordered_map<std::string, std::string> loadConfig()
    {
        std::unordered_map<std::string, std::string> result;
        if (!std::filesystem::exists(configFile))
        {
            return result;
        }
        std::string fileContent = fileUtils::readFile(configFile);
        std::string trimmed = stringUtils::trim(fileContent);
        std::vector<std::string> lines = stringUtils::split(fileContent, "\n");
        for (std::string line : lines)
        {
            size_t equalIndex = stringUtils::indexOfFirst(line, "=");
            std::string first = stringUtils::trim(line.substr(0, equalIndex));
            std::string second = stringUtils::trim(line.substr(equalIndex + 1, line.size() - 1));
            std::string &key = first;
            std::string valueInQuotes = stringUtils::getBlock(second, "\"", "\"");
            std::string value = valueInQuotes.substr(1, valueInQuotes.size() - 2);
            result[key] = value;
        }
        return result;
    };

    void saveConfig(std::unordered_map<std::string, std::string> config)
    {
        std::string output = "";
        for (const auto &pair : config)
        {
            output += pair.first + " = \"" + pair.second + "\"\n";
        }
        fileUtils::writeToFile(configFile, output);
    };
}