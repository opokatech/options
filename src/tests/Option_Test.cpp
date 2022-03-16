#include "catch2/catch_test_macros.hpp"

#include "options/Option.hpp"

TEST_CASE("Option")
{
    SECTION("Simple initialization without short option")
    {
        Options::Option o("long_name", "description");

        REQUIRE(o.long_name() == "long_name");
        REQUIRE(o.description() == "description");
        REQUIRE(o.short_name() == Options::Option::SHORT_NOT_USED);
        REQUIRE_FALSE(o.has_argument());
        REQUIRE_FALSE(o.is_mandatory());
        REQUIRE_FALSE(o.has_default_value());
        REQUIRE_FALSE(o.was_found());
    }

    SECTION("Simple initialization with short option")
    {
        Options::Option o("long_name", 'l', "description");

        REQUIRE(o.long_name() == "long_name");
        REQUIRE(o.description() == "description");
        REQUIRE(o.short_name() == 'l');
        REQUIRE_FALSE(o.has_argument());
        REQUIRE_FALSE(o.is_mandatory());
        REQUIRE_FALSE(o.has_default_value());
        REQUIRE_FALSE(o.was_found());

        SECTION("Using default value for not found option")
        {
            o.default_value("bla");
            REQUIRE(o.has_default_value());
            REQUIRE_FALSE(o.was_found());
            REQUIRE(o.as_string() == "bla");

            o.default_value("10");
            REQUIRE(o.has_default_value());
            REQUIRE(o.as_string() == "10");
            REQUIRE(o.as_int() == 10);
        }
    }

    SECTION("Setting value")
    {
        Options::Option o("opt", "some description");

        o.default_value("bla");
        REQUIRE_FALSE(o.was_found());
        REQUIRE(o.as_string() == "bla"); // was not found, so returning default

        REQUIRE(o.set_value("whatever"));
        REQUIRE(o.was_found());
        REQUIRE(o.as_string() == "whatever"); // found, so returning found-value
    }

    SECTION("Using validator")
    {
        Options::Option o("opt", "valid are strings: hello, world");

        o.validator([](const std::string &v) { return v == "hello" || v == "world"; });

        REQUIRE_FALSE(o.was_found());
        REQUIRE(o.as_string().empty());

        REQUIRE_FALSE(o.set_value("whatever"));
        REQUIRE_FALSE(o.was_found());
        REQUIRE(o.as_string().empty());

        REQUIRE(o.set_value("hello"));
        REQUIRE(o.was_found());
        REQUIRE(o.as_string() == "hello");
    }
}
