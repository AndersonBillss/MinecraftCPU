#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "assembler/assembler.hpp"
#include "schematicBuilder/schematicBuilder.hpp"
#include "utils/stringUtils.hpp"
#include "utils/binStringUtils.hpp"
#include "utils/fileUtils.hpp"
#include "utils/syntaxError.hpp"

void handleAssembleArg(const int &argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "No input file path provided" << std::endl;
        exit(1);
    }
    std::string inFileName = argv[2];
    std::string outFileName;
    size_t startingIndex = 3;
    for (int i = startingIndex; i < argc; i++)
    {
        if (std::string(argv[i]) == "-o")
        {
            if (i + 1 < argc - 1)
            {
                std::cerr << "There must be an argument after '-o'" << std::endl;
                exit(1);
            }
            outFileName = argv[i + 1];
            if (outFileName[0] == '-')
            {
                std::cerr << "file input cannot start with '-'" << std::endl;
                exit(1);
            }
        }
    }
    if (inFileName.empty())
    {
        std::cerr << "No input file path provided" << std::endl;
        exit(1);
    }
    if (outFileName.empty())
    {
        std::cerr << "No output file path provided" << std::endl;
        exit(1);
    }
    std::string sourceCode = fileUtils::readFile(inFileName);
    std::string assembled;
    try
    {
        assembled = Assembler::compile(sourceCode);
    }
    catch (const SyntaxError &e)
    {
        std::cerr << "Compilation error found in '" + outFileName + "'\n -> " + e.what() << std::endl;
        exit(1);
    }

    try
    {
        fileUtils::writeToFile(outFileName, assembled);
    }
    catch (const SyntaxError &e)
    {
        std::cerr << std::string(e.what()) + std::string(":\n -> Provided file path: '") + stringUtils::trim(outFileName) + "'" << std::endl;
        exit(1);
    }
}

// no flags (empty)
//      -o
//      -o-schem
//
// -compile
//      -o
//
// -assemble
//      -o
//      -o-schem
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "No Arguments provided\nUse the -help flag to see available commands" << std::endl;
        return 1;
    }

    if (std::string(argv[1]) == "-compile")
    {
        std::cout << "Compile is not implemented yet" << std::endl;
        return 0;
    }
    else if (std::string(argv[1]) == "-assemble")
    {
        handleAssembleArg(argc, argv);
    }
    else
    {
        std::cout << "Default command is not implemented yet" << std::endl;
        return 0;
    }
    return 0;
}