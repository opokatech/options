#include "catch2/catch_test_macros.hpp"

#include "options/Options.hpp"

TEST_CASE("Options")
{
    Options::Options o;

    SECTION("Not expecting any parameters and nothing given")
    {
        const char *argv[] = {"prg"};
        const size_t argc = sizeof(argv) / sizeof(char *);

        REQUIRE(o.parse(argc, argv));
        REQUIRE(o.positional_count() == 0);
    }

    SECTION("Parsing")
    {
        o.add_mandatory("mode", "Operation mode. Valid are: fast, slow.",
                        [](const std::string &v) { return (v == "fast" || v == "slow"); });

        o.add_optional("opt", 'o', "Bla bla", "");
        o.add_flag("only_long", "flag with only long option visible");
        o.add_flag("verbose", 'v', "flag with both short and long option");

        REQUIRE(o.positional_count() == 0);
        REQUIRE_THROWS(o.positional(0) == "a");
        REQUIRE_THROWS(o.positional(1) == "a");

        SECTION("Missing mandatory")
        {
            const char *argv[] = {"prg", "-v"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(o.parse(argc, argv));
        }

        SECTION("Wrong mandatory")
        {
            const char *argv[] = {"prg", "-v", "--mode", "bla"};
            REQUIRE_FALSE(o.parse(sizeof(argv) / sizeof(char *), argv));
        }

        SECTION("Empty mandatory")
        {
            const char *argv[] = {"prg", "-v", "--mode"};
            REQUIRE_FALSE(o.parse(sizeof(argv) / sizeof(char *), argv));
        }

        SECTION("Valid mandatory")
        {
            const char *argv[] = {"prg", "--mode", "slow"};
            REQUIRE(o.parse(sizeof(argv) / sizeof(char *), argv));

            REQUIRE(o.as_string("mode") == "slow");
            // the other parameters have some default values
            REQUIRE(o.as_string("opt").empty());
            REQUIRE_FALSE(o.as_bool("only_long"));
            REQUIRE_FALSE(o.as_bool("verbose"));
        }

        SECTION("Option not found")
        {
            REQUIRE_THROWS(o.as_bool("non_existing"));
        }

        SECTION("More parameters")
        {
            const char *argv[] = {"prg",         "--mode", "fast",     "-o", "false",
                                  "--only_long", "--",     "whatever", "is", "here"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE(o.parse(argc, argv));

            REQUIRE(o.as_string("mode") == "fast");
            REQUIRE_FALSE(o.as_bool("verbose"));
            REQUIRE(o.as_string("opt") == "false");
            REQUIRE(o.as_bool("opt") == false);
            REQUIRE(o.as_bool("only_long"));

            REQUIRE(o.positional_count() == 3);
            REQUIRE(o.positional(0) == "whatever");
            REQUIRE(o.positional(1) == "is");
            REQUIRE(o.positional(2) == "here");
        }
    }

    SECTION("parsing flags and optional arguments")
    {
        o.add_flag("verbose", "Verbose");
        o.add_flag("debug", 'd', "Debug flag");

        o.add_optional("mode", "Operating mode", "fake");
        o.add_optional("color", 'c', "Color selection", "none");
        o.add_optional("speed", "Speed selection", "slow",
                       [](const std::string &v) { return v == "slow" || v == "fast"; });
        o.add_optional("height", 'h', "Height", "low", [](const std::string &v) { return v == "low" || v == "high"; });

        SECTION("no parameters")
        {
            const char *argv[] = {"prg"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE(o.parse(argc, argv));

            REQUIRE_FALSE(o.as_bool("verbose"));
            REQUIRE_FALSE(o.as_bool("debug"));

            REQUIRE(o.as_string("mode") == "fake");
            REQUIRE(o.as_string("color") == "none");
            REQUIRE(o.as_string("speed") == "slow");
            REQUIRE(o.as_string("height") == "low");
        }

        SECTION("setting flags")
        {
            const char *argv[] = {"prg", "--verbose", "-d"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE(o.parse(argc, argv));

            REQUIRE(o.as_bool("verbose"));
            REQUIRE(o.as_bool("debug"));
        }

        SECTION("setting optional without validator without value fails")
        {
            const char *argv[] = {"prg", "--mode"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(o.parse(argc, argv));
        }

        SECTION("setting optional without validator with any value is OK")
        {
            const char *argv[] = {"prg", "--mode", "some_mode"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE(o.parse(argc, argv));
            REQUIRE(o.as_string("mode") == "some_mode");
        }

        SECTION("setting optional without validator with any value via short name is OK")
        {
            const char *argv[] = {"prg", "-c", "blue"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE(o.parse(argc, argv));
            REQUIRE(o.as_string("color") == "blue");
        }

        SECTION("setting optional with validator with invalid value fails")
        {
            const char *argv[] = {"prg", "--speed", "very_fast"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(o.parse(argc, argv));
        }

        SECTION("setting optional with validator with valid value is OK")
        {
            const char *argv[] = {"prg", "--speed", "fast"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE(o.parse(argc, argv));
            REQUIRE(o.as_string("speed") == "fast");
        }

        SECTION("setting optional with validator with invalid value via short name fails")
        {
            const char *argv[] = {"prg", "-h", "something"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(o.parse(argc, argv));
        }

        SECTION("setting optional with validator with valid value via short name is OK")
        {
            const char *argv[] = {"prg", "-h", "high"};
            const size_t argc = sizeof(argv) / sizeof(char *);

            REQUIRE(o.parse(argc, argv));
            REQUIRE(o.as_string("height") == "high");
        }
    }
}
