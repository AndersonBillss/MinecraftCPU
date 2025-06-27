#include <fstream>
#include <string>

namespace fileUtils
{
    std::ofstream createFile(std::string outputPath);
    std::string readFile(const std::string &filename);
    void writeToFile(const std::string &outputPath, std::string &contents);
}