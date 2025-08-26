#include "catch_amalgamated.hpp"
#include "../src/CLI/cli.hpp"
#include <string>
#include <vector>

struct Argv
{
    std::vector<std::string> storage;
    std::vector<char *> ptrs;

    Argv(std::initializer_list<std::string> args) : storage(args)
    {
        for (auto &s : storage)
        {
            ptrs.push_back(s.data());
        }
        ptrs.push_back(nullptr);
    }

    int argc() const { return static_cast<int>(storage.size()); }
    char **argv() { return ptrs.data(); }
};

Cli::Parsed parseHelper(std::initializer_list<std::string> args, std::function<void(Cli::Options &)> setup)
{
    Cli::Options options;
    setup(options);
    Argv argv(args);
    return options.parse(argv.argc(), argv.argv());
}

TEST_CASE("CLI flags")
{
    Cli::Parsed parsed;

    parsed = parseHelper({"mcScript", "--sampleFlag"}, [](Cli::Options &o)
                         { o.boolOption("sampleFlag", "s")
                               .addHelp("Wow some sample options"); });
    REQUIRE(parsed.count("sampleFlag") == 1);

    parsed = parseHelper({"mcScript", "-s"}, [](Cli::Options &o)
                         { o.boolOption("sampleFlag", "s")
                               .addHelp("Wow some sample options"); });
    REQUIRE(parsed.count("sampleFlag") == 1);

    parsed = parseHelper({"mcScript", "-s"}, [](Cli::Options &o)
                         { o.boolOption("sampleFlag", "s")
                               .addHelp("Wow some sample options"); });
    REQUIRE(parsed.get<bool>("sampleFlag") == true);

    parsed = parseHelper({"mcScript", "-s"}, [](Cli::Options &o)
                         { o.boolOption("sampleFlag", "s")
                                   .addHelp("Wow some sample options");
                            o.stringOption("text"); });
    REQUIRE(parsed.get<bool>("sampleFlag") == true);
}