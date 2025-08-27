#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "assembler/assembler.hpp"
#include "schematicBuilder/schematicBuilder.hpp"
#include "configManager/configManager.hpp"
#include "utils/stringUtils.hpp"
#include "utils/binStringUtils.hpp"
#include "utils/fileUtils.hpp"
#include "utils/syntaxError.hpp"
#include "CLI/cli.hpp"

void handleAssembleArg(Cli::Parsed parsed)
{
    if (!parsed.count("assemble"))
    {
        std::cerr << "No input filepath provided" << std::endl;
        exit(1);
    }
    std::string inFilePath = parsed.get<std::string>("assemble");
    bool shouldOutputMcexe = parsed.count("output-mcexe");
    bool shouldOutputSchem = parsed.count("output-schem");
    std::string outMcexePath;
    std::string outSchemPath;
    if (shouldOutputMcexe)
    {
        outMcexePath = parsed.get<std::string>("output-mcexe");
    }
    if (shouldOutputSchem)
    {
        outSchemPath = parsed.get<std::string>("output-schem");
    }
    if (!shouldOutputMcexe && !shouldOutputSchem)
    {
        std::cerr << "No output option provided" << std::endl;
        exit(0);
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

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "No Arguments provided\nUse the --help flag to see available commands" << std::endl;
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

    Cli::Options options("McScript", "A tool for compiling and emulating code that runs on a custom Minecraft computer");
    options.stringOption("compile", "c").addHelp("Compile .mcscript code into assembly (not implemented yet)");
    options.stringOption("assemble", "a").addHelp("Assemble .mcasm assembly code");
    options.stringOption("execute", "a").addHelp("Execute .mcexe binary via an emulator (not implemented yet)");
    options.stringOption("set-schem-path").addHelp("Set a default schematic ouput path");
    options.stringOption("set-mcexe-path").addHelp("Set a default mcexe ouput path");
    options.boolOption("help", "h").addHelp("Print usage");
    options.stringOption("output-mcexe", "o").addHelp("Filepath for the output .mcexe file").addImplicit(defaultMcexePath);
    options.stringOption("output-schem", "s").addHelp("Filepath for the output schematic file").addImplicit(defaultSchemPath);

    Cli::Parsed parsed;
    try
    {
        parsed = options.parse(argc, argv);
        parsed.ensureExclusive({"compile", "assemble", "execute", "help", "set-schem-path", "set-mcexe-path"});
    }
    catch (CliError &e)
    {
        std::cerr << e.what() << std::endl;
        exit(0);
    }

    if (parsed.count("help"))
    {
        options.printHelp();
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
        std::string path = parsed.get<std::string>("set-schem-path");
        config["schem-path"] = path;
        configManager::saveConfig(config);
        std::cout << "Schematic path is now set to \"" + path + "\"" << std::endl;
        exit(0);
    }
    if (parsed.count("set-mcexe-path"))
    {
        std::string path = parsed.get<std::string>("set-mcexe-path");
        config["mcexe-path"] = path;
        configManager::saveConfig(config);
        std::cout << "mcexe path is now set to \"" + path + "\"" << std::endl;
        exit(0);
    }
    std::cout << "Default command is not implemented yet" << std::endl;
    return 0;
}