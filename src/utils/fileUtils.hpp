#include <fstream>
#include <string>

namespace fileUtils
{
    std::string readFile(const std::string &filename);
    void writeToFile(const std::string &outputPath, const std::string &contents);
}