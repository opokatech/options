#include "catch2/catch_test_macros.hpp"

#include "options/Parser.hpp"

TEST_CASE("Parser")
{
    Options::Parser parser;

    SECTION("Not expecting any parameters and nothing given")
    {
        const char *argv[] = {"prg"};
        const size_t ARGC = sizeof(argv) / sizeof(char *);

        REQUIRE(parser.parse(ARGC, argv));
        REQUIRE(parser.positional_count() == 0);
    }

    SECTION("Parsing")
    {
        parser.add_mandatory("mode", "Operation mode. Valid are: fast, slow.",
                             [](const std::string &value) { return (value == "fast" || value == "slow"); });

        parser.add_optional("opt", 'o', "Bla bla", "");
        parser.add_flag("only_long", "flag with only long option visible");
        parser.add_flag("verbose", 'v', "flag with both short and long option");

        REQUIRE(parser.positional_count() == 0);
        REQUIRE_THROWS(parser.positional(0) == "a");
        REQUIRE_THROWS(parser.positional(1) == "a");

        SECTION("Getting possible options")
        {
            const auto possible_options = parser.get_possible_options();

            // only check if the options are there
            REQUIRE(possible_options.find("--mode ") != std::string::npos);
            REQUIRE(possible_options.find("--opt ") != std::string::npos);
            REQUIRE(possible_options.find("--only_long ") != std::string::npos);
            REQUIRE(possible_options.find("--verbose ") != std::string::npos);
            REQUIRE(possible_options.find("--non-existing ") == std::string::npos);
        }

        SECTION("Missing mandatory")
        {
            const char *argv[] = {"prg", "-v"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(parser.parse(ARGC, argv));
        }

        SECTION("Wrong mandatory")
        {
            const char *argv[] = {"prg", "-v", "--mode", "bla"};
            REQUIRE_FALSE(parser.parse(sizeof(argv) / sizeof(char *), argv));
        }

        SECTION("Empty mandatory")
        {
            const char *argv[] = {"prg", "-v", "--mode"};
            REQUIRE_FALSE(parser.parse(sizeof(argv) / sizeof(char *), argv));
        }

        SECTION("Valid mandatory")
        {
            const char *argv[] = {"prg", "--mode", "slow"};
            REQUIRE(parser.parse(sizeof(argv) / sizeof(char *), argv));

            REQUIRE(parser.as_string("mode") == "slow");
            // the other parameters have some default values
            REQUIRE(parser.as_string("opt").empty());
            REQUIRE_FALSE(parser.as_bool("only_long"));
            REQUIRE_FALSE(parser.as_bool("verbose"));
        }

        SECTION("Option not found")
        {
            REQUIRE_THROWS(parser.as_bool("non_existing"));
        }

        SECTION("More parameters")
        {
            const char *argv[] = {"prg",         "--mode", "fast",     "-o", "false",
                                  "--only_long", "--",     "whatever", "is", "here"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(parser.parse(ARGC, argv));

            REQUIRE(parser.as_string("mode") == "fast");
            REQUIRE_FALSE(parser.as_bool("verbose"));
            REQUIRE(parser.as_string("opt") == "false");
            REQUIRE(parser.as_bool("opt") == false);
            REQUIRE(parser.as_bool("only_long"));
            REQUIRE_THROWS(parser.as_string("non-existing").empty());

            REQUIRE(parser.positional_count() == 3);
            REQUIRE(parser.positional(0) == "whatever");
            REQUIRE(parser.positional(1) == "is");
            REQUIRE(parser.positional(2) == "here");
        }
    }

    SECTION("parsing flags and optional arguments")
    {
        parser.add_flag("verbose", "Verbose");
        parser.add_flag("debug", 'd', "Debug flag");

        parser.add_optional("mode", "Operating mode", "fake");
        parser.add_optional("color", 'c', "Color selection", "none");
        parser.add_optional("speed", "Speed selection", "slow",
                            [](const std::string &value) { return value == "slow" || value == "fast"; });
        parser.add_optional("height", 'h', "Height", "low",
                            [](const std::string &value) { return value == "low" || value == "high"; });
        parser.add_optional("integer", "Any integer signed or not", "0");
        parser.add_optional("float", "Any floating point value", "0");

        SECTION("no parameters")
        {
            const char *argv[] = {"prg"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(parser.parse(ARGC, argv));

            REQUIRE_FALSE(parser.as_bool("verbose"));
            REQUIRE_FALSE(parser.as_bool("debug"));

            REQUIRE(parser.as_string("mode") == "fake");
            REQUIRE(parser.as_string("color") == "none");
            REQUIRE(parser.as_string("speed") == "slow");
            REQUIRE(parser.as_string("height") == "low");
            REQUIRE(parser.as_string("integer") == "0");
            REQUIRE(parser.as_string("float") == "0");

            REQUIRE(parser.as_int("integer") == 0);
            REQUIRE(parser.as_uint("integer") == 0);
            REQUIRE(parser.as_double("float") == 0.0);
        }

        SECTION("setting flags")
        {
            const char *argv[] = {"prg", "--verbose", "-d"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(parser.parse(ARGC, argv));

            REQUIRE(parser.as_bool("verbose"));
            REQUIRE(parser.as_bool("debug"));
        }

        SECTION("setting optional without validator without value fails")
        {
            const char *argv[] = {"prg", "--mode"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(parser.parse(ARGC, argv));
        }

        SECTION("setting optional without validator with any value is OK")
        {
            const char *argv[] = {"prg", "--mode", "some_mode", "--integer", "42", "--float", "3.14"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(parser.parse(ARGC, argv));
            REQUIRE(parser.as_string("mode") == "some_mode");

            REQUIRE(parser.as_int("integer") == 42);
            REQUIRE(parser.as_uint("integer") == 42);
            REQUIRE(parser.as_double("float") == 3.14);
        }

        SECTION("setting optional without validator with any value via short name is OK")
        {
            const char *argv[] = {"prg", "-c", "blue"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(parser.parse(ARGC, argv));
            REQUIRE(parser.as_string("color") == "blue");
        }

        SECTION("setting optional with validator with invalid value fails")
        {
            const char *argv[] = {"prg", "--speed", "very_fast"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(parser.parse(ARGC, argv));
        }

        SECTION("setting optional with validator with valid value is OK")
        {
            const char *argv[] = {"prg", "--speed", "fast"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(parser.parse(ARGC, argv));
            REQUIRE(parser.as_string("speed") == "fast");
        }

        SECTION("setting optional with validator with invalid value via short name fails")
        {
            const char *argv[] = {"prg", "-h", "something"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE_FALSE(parser.parse(ARGC, argv));
        }

        SECTION("setting optional with validator with valid value via short name is OK")
        {
            const char *argv[] = {"prg", "-h", "high"};
            const size_t ARGC = sizeof(argv) / sizeof(char *);

            REQUIRE(parser.parse(ARGC, argv));
            REQUIRE(parser.as_string("height") == "high");
        }
    }
}
