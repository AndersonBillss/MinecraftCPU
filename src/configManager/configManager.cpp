#include "configManager.hpp"
#include "../utils/fileUtils.hpp"
#include "../utils/stringUtils.hpp"
#include <filesystem>

const std::string configFile = ".config";

namespace ConfigManager
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
        return result;
    };
    void saveConfig(std::unordered_map<std::string, std::string> config) {

    };
}