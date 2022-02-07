#include "catch2/catch_test_macros.hpp"

#include "options/Options.hpp"

TEST_CASE("Options")
{
    Options::Options o;

    SECTION("Not expecting any parameters and nothing given")
    {
        const char *argv[] = {"prg"};
        REQUIRE(o.parse(sizeof(argv) / sizeof(char *), argv));
        REQUIRE(o.positional_count() == 0);
    }

    SECTION("Parsing")
    {
        o.add({"mode", "Operation mode. Valid are: fast, slow."})
            .argument()
            .mandatory()
            .validator([](const std::string &v) { return (v == "fast" || v == "slow"); });

        o.add({"mode2", 'm', "Bla bla"}).argument();
        o.add({"only_long", "flag with only long option visible"});
        o.add({"verbose", 'v', "flag with both short and long option"});

        REQUIRE(o.positional_count() == 0);
        REQUIRE_THROWS(o.positional(0) == "a");
        REQUIRE_THROWS(o.positional(1) == "a");

        SECTION("Missing mandatory")
        {
            const char *argv[] = {"prg", "-v"};
            REQUIRE_FALSE(o.parse(sizeof(argv) / sizeof(char *), argv));
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
            REQUIRE(o.as_string("mode2").empty());
            REQUIRE_FALSE(o.as_bool("only_long"));
            REQUIRE_FALSE(o.as_bool("verbose"));
        }

        SECTION("Option not found") { REQUIRE_THROWS(o.as_bool("non_existing")); }

        SECTION("More parameters")
        {
            const char *argv[] = {"prg",         "--mode", "fast",     "-m", "false",
                                  "--only_long", "--",     "whatever", "is", "here"};
            REQUIRE(o.parse(sizeof(argv) / sizeof(char *), argv));

            REQUIRE(o.as_string("mode") == "fast");
            REQUIRE_FALSE(o.as_bool("verbose"));
            REQUIRE(o.as_string("mode2") == "false");
            REQUIRE(o.as_bool("mode2") == false);
            REQUIRE(o.as_bool("only_long"));

            REQUIRE(o.positional_count() == 3);
            REQUIRE(o.positional(0) == "whatever");
            REQUIRE(o.positional(1) == "is");
            REQUIRE(o.positional(2) == "here");
        }
    }
}
