#pragma once

#include <cstdint>
#include <string>

#include "Validator.hpp"

namespace Options
{
    /* Single Option representation.
     *
     * An option can be either:
     *  - a Flag (default) - which can be set or not,
     *  - a Mandatory - which requires a parameter,
     *  - an Optional - which takes a parameter, but has some default.
     *
     * A validator can be set which act as a filter regarding acceptable values.
     *
     * A value can be retrieved as int, uint, double, bool or string (which is the default type).
     */
    class Option
    {
    public:
        Option(const std::string &long_name, char short_name, const std::string &description);
        Option(const std::string &long_name, const std::string &description);

        Option &set_mandatory();
        Option &set_optional(const std::string &default_value);

        Option &set_validator(validator_t v);
        bool    set_value(const std::string &v);

        char               short_name() const { return _short_name; }
        const std::string &long_name() const { return _long_name; }
        const std::string &description() const { return _description; }
        const std::string &default_value() const { return _default_value; }

        bool is_flag() const { return _type == Type::Flag; }
        bool is_mandatory() const { return _type == Type::Mandatory; }
        bool is_optional() const { return _type == Type::Optional; }
        bool has_argument() const { return is_mandatory() || is_optional(); }
        bool was_set() const { return _was_set; }

        int32_t            as_int() const;
        uint32_t           as_uint() const;
        double             as_double() const;
        bool               as_bool() const;
        const std::string &as_string() const;

        static constexpr char SHORT_NOT_USED = 0;

    private:
        enum class Type
        {
            Flag,     // simple flag without parameters
            Optional, // takes optional parameter and requires default
            Mandatory // takes mandatory parameter (so no default)
        };

        char        _short_name = SHORT_NOT_USED;
        std::string _long_name;
        std::string _description;
        validator_t _validator = nullptr;
        Type        _type      = Type::Flag;
        std::string _default_value;

        bool        _was_set = false;
        std::string _value;
    };
} // namespace Options
