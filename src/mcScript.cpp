#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "assembler/assembler.hpp"
#include "utils/stringUtils.hpp"
#include "utils/binUtils.hpp"
#include "utils/syntaxError.hpp"
#include "utils/fileUtils.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "No file provided" << std::endl;
        return 1;
    }
    std::string fileName = argv[1];
    std::string sourceCode = fileUtils::readFile(fileName);

    std::string assembled;
    try
    {
        assembled = Assembler::compile(sourceCode);
    }
    catch (const SyntaxError &e)
    {
        std::cerr << "Compilation error found in '" + fileName + "'\n -> " + e.what() << std::endl;
        return 1;
    }

    // std::cout << assembled << std::endl;


    fileUtils::writeToFile("sampleFile/hello/test.txt", "HELLO, WORLD!");
/*     std::ofstream outfile(outFilePath);
    if(!outfile.is_open()){
        std::cerr << "Failed to write to output file" << std::endl;
        return 1;
    }
    outfile << assembled;
 */

    
    return 0;
}