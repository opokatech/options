#include "catch2/catch_test_macros.hpp"

#include "options/Options.hpp"

TEST_CASE("Options")
{
    Options::Options options;

    SECTION("Not expecting any parameters and nothing given")
    {
        const char *argv[] = {"prg"};
        const size_t ARGC = sizeof(argv) / sizeof(char *);

        REQUIRE(options.parse(ARGC, argv));
        REQUIRE(options.positional_count() == 0);
    }

    SECTION("Parsing")
    {
        options.add_mandatory("mode", "Operation mode. Valid are: fast, slow.",
                              [](const std::string &value) { return (value == "fast" || value == "slow"); });

        options.add_optional("opt", 'o', "Bla bla", "");
        options.add_flag("only_long", "flag with only long option visible");
        options.add_flag("verbose", 'v', "flag with both short and long option");

        REQUIRE(options.positional_count() == 0);
        REQUIRE_THROWS(options.positional(0) == "a");
        REQUIRE_THROWS(options.positional(1) == "a");

        SECTION("Missing mandatory")
        {
            const char *argv[] = {"prg", "-v"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(options.parse(ARGC, argv));
        }

        SECTION("Wrong mandatory")
        {
            const char *argv[] = {"prg", "-v", "--mode", "bla"};
            REQUIRE_FALSE(options.parse(sizeof(argv) / sizeof(char *), argv));
        }

        SECTION("Empty mandatory")
        {
            const char *argv[] = {"prg", "-v", "--mode"};
            REQUIRE_FALSE(options.parse(sizeof(argv) / sizeof(char *), argv));
        }

        SECTION("Valid mandatory")
        {
            const char *argv[] = {"prg", "--mode", "slow"};
            REQUIRE(options.parse(sizeof(argv) / sizeof(char *), argv));

            REQUIRE(options.as_string("mode") == "slow");
            // the other parameters have some default values
            REQUIRE(options.as_string("opt").empty());
            REQUIRE_FALSE(options.as_bool("only_long"));
            REQUIRE_FALSE(options.as_bool("verbose"));
        }

        SECTION("Option not found")
        {
            REQUIRE_THROWS(options.as_bool("non_existing"));
        }

        SECTION("More parameters")
        {
            const char *argv[] = {"prg",         "--mode", "fast",     "-o", "false",
                                  "--only_long", "--",     "whatever", "is", "here"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(options.parse(ARGC, argv));

            REQUIRE(options.as_string("mode") == "fast");
            REQUIRE_FALSE(options.as_bool("verbose"));
            REQUIRE(options.as_string("opt") == "false");
            REQUIRE(options.as_bool("opt") == false);
            REQUIRE(options.as_bool("only_long"));

            REQUIRE(options.positional_count() == 3);
            REQUIRE(options.positional(0) == "whatever");
            REQUIRE(options.positional(1) == "is");
            REQUIRE(options.positional(2) == "here");
        }
    }

    SECTION("parsing flags and optional arguments")
    {
        options.add_flag("verbose", "Verbose");
        options.add_flag("debug", 'd', "Debug flag");

        options.add_optional("mode", "Operating mode", "fake");
        options.add_optional("color", 'c', "Color selection", "none");
        options.add_optional("speed", "Speed selection", "slow",
                             [](const std::string &value) { return value == "slow" || value == "fast"; });
        options.add_optional("height", 'h', "Height", "low",
                             [](const std::string &value) { return value == "low" || value == "high"; });

        SECTION("no parameters")
        {
            const char *argv[] = {"prg"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(options.parse(ARGC, argv));

            REQUIRE_FALSE(options.as_bool("verbose"));
            REQUIRE_FALSE(options.as_bool("debug"));

            REQUIRE(options.as_string("mode") == "fake");
            REQUIRE(options.as_string("color") == "none");
            REQUIRE(options.as_string("speed") == "slow");
            REQUIRE(options.as_string("height") == "low");
        }

        SECTION("setting flags")
        {
            const char *argv[] = {"prg", "--verbose", "-d"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(options.parse(ARGC, argv));

            REQUIRE(options.as_bool("verbose"));
            REQUIRE(options.as_bool("debug"));
        }

        SECTION("setting optional without validator without value fails")
        {
            const char *argv[] = {"prg", "--mode"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(options.parse(ARGC, argv));
        }

        SECTION("setting optional without validator with any value is OK")
        {
            const char *argv[] = {"prg", "--mode", "some_mode"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(options.parse(ARGC, argv));
            REQUIRE(options.as_string("mode") == "some_mode");
        }

        SECTION("setting optional without validator with any value via short name is OK")
        {
            const char *argv[] = {"prg", "-c", "blue"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(options.parse(ARGC, argv));
            REQUIRE(options.as_string("color") == "blue");
        }

        SECTION("setting optional with validator with invalid value fails")
        {
            const char *argv[] = {"prg", "--speed", "very_fast"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(options.parse(ARGC, argv));
        }

        SECTION("setting optional with validator with valid value is OK")
        {
            const char *argv[] = {"prg", "--speed", "fast"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(options.parse(ARGC, argv));
            REQUIRE(options.as_string("speed") == "fast");
        }

        SECTION("setting optional with validator with invalid value via short name fails")
        {
            const char *argv[] = {"prg", "-h", "something"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(options.parse(ARGC, argv));
        }

        SECTION("setting optional with validator with valid value via short name is OK")
        {
            const char *argv[] = {"prg", "-h", "high"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(options.parse(ARGC, argv));
            REQUIRE(options.as_string("height") == "high");
        }
    }
}
