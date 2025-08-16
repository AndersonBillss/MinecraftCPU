#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "cxxopts.hpp"
#include "assembler/assembler.hpp"
#include "schematicBuilder/schematicBuilder.hpp"
#include "utils/stringUtils.hpp"
#include "utils/binStringUtils.hpp"
#include "utils/fileUtils.hpp"
#include "utils/syntaxError.hpp"

const std::string defaultOutputFolder = "mcScriptBuild/";
const std::string defaultOutputFileName = "bin";
const std::string defaultExecutableExtension = ".mcasm";

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
        outFileName = defaultOutputFolder + defaultOutputFileName + defaultExecutableExtension;
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

void handleSchemPathArg(const int &argc, char *argv[])
{
}

void checkExclusive(const cxxopts::ParseResult &result, const std::vector<std::string> &opts)
{
    std::vector<std::string> selectedFlags;
    for (auto &opt : opts)
    {
        if (result.count(opt))
            selectedFlags.push_back(opt);
    }
    if (selectedFlags.size() > 1)
    {
        std::cerr << "Error: options ";
        for(size_t i = 0; i < selectedFlags.size(); i++){
            std::string& flag = selectedFlags[i];
            std::string textFormat = ",";
            if(selectedFlags.size() == 2 || i == 0){
                textFormat = "";
            }
            if(i == selectedFlags.size()-1){
                textFormat += "and ";
            }
            std::cerr << textFormat << "--" << flag << " ";
        }
        std::cerr << "are mutually exclusive.\n";
        std::exit(1);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "No Arguments provided\nUse the -help flag to see available commands" << std::endl;
        return 1;
    }

    cxxopts::Options options("McScript", "A tool for compiling and emulating code that runs on a custom Minecraft computer");
    options.add_options()
        ("c,compile", "Compile .mcscript code into assembly (not implemented yet)")
        ("a,assemble", "Assemble .mcasm assembly code")
        ("x,execute", "Execute .mcexe binary via an emulator (not implemented yet)")
        ("h,help", "Print usage")
        ("of,output-file", "Filepath for the output binary", cxxopts::value<std::string>()->implicit_value("bin.mcexe"))
        ("os,output-schematic", "Filepath for the output schematic", cxxopts::value<std::string>()->implicit_value("bin.mcexe"))
    ;
    auto parsed = options.parse(argc, argv);

    const std::vector<std::string> commandTypes = {"compile", "assemble", "execute", "help"};
    checkExclusive(parsed, commandTypes);

    if (std::string(argv[1]) == "-compile")
    {
        std::cout << "Compile is not implemented yet" << std::endl;
        return 0;
    }
    else if (std::string(argv[1]) == "-assemble")
    {
        handleAssembleArg(argc, argv);
    }
    else if (std::string(argv[1]) == "-set-schem-path")
    {
        handleSchemPathArg(argc, argv);
    }
    else
    {
        std::cout << "Default command is not implemented yet" << std::endl;
        return 0;
    }
    return 0;
}