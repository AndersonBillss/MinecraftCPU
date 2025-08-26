#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "cxxopts.hpp"
#include "assembler/assembler.hpp"
#include "schematicBuilder/schematicBuilder.hpp"
#include "configManager/configManager.hpp"
#include "utils/stringUtils.hpp"
#include "utils/binStringUtils.hpp"
#include "utils/fileUtils.hpp"
#include "utils/syntaxError.hpp"

void handleAssembleArg(cxxopts::ParseResult parsed)
{
    if (!parsed.count("assemble"))
    {
        std::cerr << "No input filepath provided" << std::endl;
        exit(1);
    }
    std::string inFilePath = parsed["assemble"].as<std::string>();
    bool shouldOutputMcexe = parsed.count("output-mcexe");
    bool shouldOutputSchem = parsed.count("output-schem");
    std::string outMcexePath;
    std::string outSchemPath;
    if (shouldOutputMcexe)
    {
        outMcexePath = parsed["output-mcexe"].as<std::string>();
    }
    if (shouldOutputSchem)
    {
        outSchemPath = parsed["output-schem"].as<std::string>();
    }
    if (!shouldOutputMcexe && !shouldOutputSchem)
    {
        std::cout << "No output option provided" << std::endl;
    }

    std::string sourceCode = fileUtils::readFile(inFilePath);
    std::string assembled;
    try
    {
        assembled = Assembler::compile(sourceCode);
    }
    catch (const SyntaxError &e)
    {
        std::cerr << "Compilation error found in '" + outMcexePath + "'\n -> " + e.what() << std::endl;
        exit(1);
    }

    if (shouldOutputMcexe)
    {
        fileUtils::writeToFile(outMcexePath, assembled);
    }
    if (shouldOutputSchem)
    {
        schematicBuilder::writeToFile(outSchemPath, assembled);
    }
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
        for (size_t i = 0; i < selectedFlags.size(); i++)
        {
            std::string &flag = selectedFlags[i];
            std::string textFormat = ",";
            if (selectedFlags.size() == 2 || i == 0)
            {
                textFormat = "";
            }
            if (i == selectedFlags.size() - 1)
            {
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
    for (int i = 0; i < argc; i++)
    {
        std::cout << argv[i] << std::endl;
    }
    return (0);

    if (argc < 2)
    {
        std::cerr << "No Arguments provided\nUse the -help flag to see available commands" << std::endl;
        return 1;
    }

    // Load config
    std::string defaultSchemPath = "mcScriptBuild/bin.schem";
    std::string defaultMcexePath = "mcScriptBuild/bin.mcexe";
    auto config = configManager::loadConfig();
    auto it = config.find("schem-path");
    if (it != config.end())
        defaultSchemPath = config["schem-path"];
    it = config.find("mcexe-path");
    if (it != config.end())
        defaultMcexePath = config["mcexe-path"];

    cxxopts::Options options("McScript", "A tool for compiling and emulating code that runs on a custom Minecraft computer");
    // clang-format off
    options.add_options()
        ("c,compile", "Compile .mcscript code into assembly (not implemented yet)", cxxopts::value<std::string>())
        ("a,assemble", "Assemble .mcasm assembly code", cxxopts::value<std::string>())
        ("x,execute", "Execute .mcexe binary via an emulator (not implemented yet)")
        ("set-schem-path", "Set a default schematic ouput path", cxxopts::value<std::string>())
        ("set-mcexe-path", "Set a default mcexe ouput path", cxxopts::value<std::string>())
        ("h,help", "Print usage")
        ("o,output-mcexe", "Filepath for the output .mcexe file", cxxopts::value<std::string>()->implicit_value(defaultMcexePath))
        ("s,output-schem", "Filepath for the output schematic", cxxopts::value<std::string>()->implicit_value(defaultSchemPath))
    ;
    // clang-format on
    cxxopts::ParseResult parsed;
    try
    {
        parsed = options.parse(argc, argv);
    }
    catch (const cxxopts::exceptions::no_such_option &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
    catch (cxxopts::exceptions::missing_argument &e)
    {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    const std::vector<std::string> commandTypes = {
        "compile",
        "assemble",
        "execute",
        "help",
        "set-schem-path",
        "set-mcexe-path"};

    checkExclusive(parsed, commandTypes);

    if (parsed.count("help"))
    {
        std::cout << options.help() << std::endl;
        exit(0);
    }
    if (parsed.count("compile"))
    {
        std::cout << "Compile is not implemented yet" << std::endl;
        exit(0);
    }
    if (parsed.count("assemble"))
    {
        handleAssembleArg(parsed);
        exit(0);
    }
    if (parsed.count("set-schem-path"))
    {
        std::string path = parsed["set-schem-path"].as<std::string>();
        config["schem-path"] = path;
        configManager::saveConfig(config);
        std::cout << "Schematic path is now set to \"" + path + "\"";
        exit(0);
    }
    if (parsed.count("set-mcexe-path"))
    {
        std::string path = parsed["set-mcexe-path"].as<std::string>();
        config["mcexe-path"] = path;
        configManager::saveConfig(config);
        std::cout << "mcexe path is now set to \"" + path + "\"";
        exit(0);
    }
    std::cout << "Default command is not implemented yet" << std::endl;
    return 0;
}