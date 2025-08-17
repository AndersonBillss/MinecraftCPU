#include <unordered_map>
#include <string>

namespace configManager {
    std::unordered_map<std::string, std::string> loadConfig();
    void saveConfig(std::unordered_map<std::string, std::string> config);
}