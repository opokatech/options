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

        REQUIRE(o.as_int() == 0);
        REQUIRE(o.as_uint() == 0);
        REQUIRE(o.as_double() == 0.0);
        REQUIRE(o.as_bool() == false);
        REQUIRE(o.as_string().empty());
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

        REQUIRE(o.as_int() == 0);
        REQUIRE(o.as_uint() == 0);
        REQUIRE(o.as_double() == 0.0);
        REQUIRE(o.as_bool() == false);
        REQUIRE(o.as_string().empty());

        SECTION("Using default textual value for not found option")
        {
            o.takes_optional_argument_with_default("bla");

            REQUIRE(o.has_default_value());
            REQUIRE_FALSE(o.was_found());

            REQUIRE(o.as_int() == 0);
            REQUIRE(o.as_uint() == 0);
            REQUIRE(o.as_double() == 0.0);
            REQUIRE(o.as_bool() == false);
            REQUIRE(o.as_string() == "bla");
        }

        SECTION("Using default positive int for not found option")
        {
            o.takes_optional_argument_with_default("5");

            REQUIRE(o.has_default_value());
            REQUIRE_FALSE(o.was_found());

            REQUIRE(o.as_int() == 5);
            REQUIRE(o.as_uint() == 5);
            REQUIRE(o.as_double() == 5.0);
            REQUIRE(o.as_bool() == true); // non-zero value
            REQUIRE(o.as_string() == "5");
        }

        SECTION("Using default double for not found option")
        {
            o.takes_optional_argument_with_default("3.14");

            REQUIRE(o.has_default_value());
            REQUIRE_FALSE(o.was_found());

            REQUIRE(o.as_int() == 3);
            REQUIRE(o.as_uint() == 3);
            REQUIRE(o.as_double() == 3.14);
            REQUIRE(o.as_bool() == true);
            REQUIRE(o.as_string() == "3.14");
        }

        SECTION("Using default bool for not found option")
        {
            o.takes_optional_argument_with_default("true");

            REQUIRE(o.has_default_value());
            REQUIRE_FALSE(o.was_found());

            REQUIRE(o.as_int() == 0);
            REQUIRE(o.as_uint() == 0);
            REQUIRE(o.as_double() == 0.0);
            REQUIRE(o.as_bool() == true);
            REQUIRE(o.as_string() == "true");
        }
    }

    SECTION("Setting value")
    {
        Options::Option o("opt", "some description");

        o.takes_optional_argument_with_default("bla");
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
