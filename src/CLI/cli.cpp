#include "cli.hpp"

void test()
{
    Cli::Options options;
    options
        .stringOption("compile", "c")
        .addDefault("testPath")
        .addImplicit("testPath2")
        .addHelp("Here are some help options");
}