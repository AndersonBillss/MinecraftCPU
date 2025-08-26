#include "catch_amalgamated.hpp"
#include "../src/CLI/cli.hpp"
#include <string>

TEST_CASE("CLI flags")
{
    Cli::Options options;
    options.boolOption("sampleFlag", "s")
        .addHelp("Wow some sample options");

    char arg0[] = "--sampleFlag";
    char *argv[] = {arg0, nullptr};
    int argc = 1;
    Cli::Parsed parsed = options.parse(argc, argv);

    REQUIRE(parsed.count("sampleFlag") == 1);
}