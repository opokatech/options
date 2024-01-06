#include "catch2/catch_test_macros.hpp"

#include "options/Option.hpp"

TEST_CASE("Option")
{
    SECTION("Simple initialization without short option")
    {
        Options::Option option("long_name", "description");

        REQUIRE(option.long_name() == "long_name");
        REQUIRE(option.description() == "description");
        REQUIRE(option.short_name() == Options::Option::SHORT_NOT_USED);
        REQUIRE(option.is_flag());
        REQUIRE_FALSE(option.is_mandatory());
        REQUIRE_FALSE(option.is_optional());
        REQUIRE_FALSE(option.has_argument());
        REQUIRE_FALSE(option.was_set());

        REQUIRE(option.as_int() == 0);
        REQUIRE(option.as_uint() == 0);
        REQUIRE(option.as_double() == 0.0);
        REQUIRE(option.as_bool() == false);
        REQUIRE(option.as_string().empty());
    }

    SECTION("Simple initialization with short option")
    {
        Options::Option option("long_name", 'l', "description");

        REQUIRE(option.long_name() == "long_name");
        REQUIRE(option.description() == "description");
        REQUIRE(option.short_name() == 'l');
        REQUIRE(option.is_flag());
        REQUIRE_FALSE(option.is_mandatory());
        REQUIRE_FALSE(option.is_optional());
        REQUIRE_FALSE(option.has_argument());
        REQUIRE_FALSE(option.was_set());

        REQUIRE(option.as_int() == 0);
        REQUIRE(option.as_uint() == 0);
        REQUIRE(option.as_double() == 0.0);
        REQUIRE(option.as_bool() == false);
        REQUIRE(option.as_string().empty());

        SECTION("Using default textual value for not found option")
        {
            option.set_optional("bla");

            REQUIRE(option.is_optional());
            REQUIRE_FALSE(option.was_set());

            REQUIRE(option.as_int() == 0);
            REQUIRE(option.as_uint() == 0);
            REQUIRE(option.as_double() == 0.0);
            REQUIRE(option.as_bool() == false);
            REQUIRE(option.as_string() == "bla");
        }

        SECTION("Using default positive int for not found option")
        {
            option.set_optional("5");

            REQUIRE(option.is_optional());
            REQUIRE_FALSE(option.was_set());

            REQUIRE(option.as_int() == 5);
            REQUIRE(option.as_uint() == 5);
            REQUIRE(option.as_double() == 5.0);
            REQUIRE(option.as_bool() == true); // non-zero value
            REQUIRE(option.as_string() == "5");
        }

        SECTION("Using default double for not found option")
        {
            option.set_optional("3.14");

            REQUIRE(option.is_optional());
            REQUIRE_FALSE(option.was_set());

            REQUIRE(option.as_int() == 3);
            REQUIRE(option.as_uint() == 3);
            REQUIRE(option.as_double() == 3.14);
            REQUIRE(option.as_bool() == true);
            REQUIRE(option.as_string() == "3.14");
        }

        SECTION("Using default bool for not found option")
        {
            option.set_optional("true");

            REQUIRE(option.is_optional());
            REQUIRE_FALSE(option.was_set());

            REQUIRE(option.as_int() == 0);
            REQUIRE(option.as_uint() == 0);
            REQUIRE(option.as_double() == 0.0);
            REQUIRE(option.as_bool() == true);
            REQUIRE(option.as_string() == "true");
        }
    }

    SECTION("Setting value")
    {
        Options::Option option("opt", "some description");

        option.set_optional("bla");
        REQUIRE_FALSE(option.was_set());
        REQUIRE(option.as_string() == "bla"); // was not found, so returning default

        REQUIRE(option.set_value("whatever"));
        REQUIRE(option.was_set());
        REQUIRE(option.as_string() == "whatever"); // found, so returning found-value
    }

    SECTION("Using validator on optional option")
    {
        Options::Option option("opt", "valid are strings: hello, world");

        option.set_optional("");
        option.set_validator([](const std::string &value) { return value == "hello" || value == "world"; });

        REQUIRE_FALSE(option.was_set());
        REQUIRE(option.as_string().empty());

        REQUIRE_FALSE(option.set_value("whatever"));
        REQUIRE_FALSE(option.was_set());
        REQUIRE(option.as_string().empty());

        REQUIRE(option.set_value("hello"));
        REQUIRE(option.was_set());
        REQUIRE(option.as_string() == "hello");
    }

    SECTION("Using validator on mandatory option")
    {
        Options::Option option("opt", "valid are strings: hello, world");

        option.set_mandatory();
        option.set_validator([](const std::string &value) { return value == "hello" || value == "world"; });

        REQUIRE_FALSE(option.was_set());
        REQUIRE(option.as_string().empty());

        REQUIRE_FALSE(option.set_value("whatever"));
        REQUIRE_FALSE(option.was_set());
        REQUIRE(option.as_string().empty());

        SECTION("Setting value hello")
        {
            REQUIRE(option.set_value("hello"));
            REQUIRE(option.was_set());
            REQUIRE(option.as_string() == "hello");
        }

        SECTION("Setting value world")
        {
            REQUIRE(option.set_value("world"));
            REQUIRE(option.was_set());
            REQUIRE(option.as_string() == "world");
        }
    }
}
