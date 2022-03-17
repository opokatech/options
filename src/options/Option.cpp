#include "Option.hpp"

#include <string.h>

namespace Options
{
    Option::Option(const std::string &long_name, char short_name, const std::string &description)
        : _short_name{short_name}, _long_name{long_name}, _description{description}
    {
    }

    // ---------------------------------------------------------------------------------------------

    Option::Option(const std::string &long_name, const std::string &description)
        : Option(long_name, SHORT_NOT_USED, description)
    {
    }

    // ---------------------------------------------------------------------------------------------

    Option &Option::validator(validator_t v)
    {
        _validator = v;
        return *this;
    }

    // ---------------------------------------------------------------------------------------------

    Option &Option::takes_mandatory_argument()
    {
        _argument  = true;
        _mandatory = true;
        return *this;
    }

    // ---------------------------------------------------------------------------------------------

    Option &Option::takes_optional_argument_with_default(const std::string &default_value)
    {
        _argument      = true;
        _default_value = default_value;
        return *this;
    }

    bool Option::set_value(const std::string &v)
    {
        if (_validator != nullptr)
            if (!_validator(v))
                return false;

        _found = true;
        _value = v;
        return true;
    }

    // ---------------------------------------------------------------------------------------------

    int32_t Option::as_int(const std::string &v)
    {
        return static_cast<int32_t>(std::strtol(v.c_str(), nullptr, 10));
    }

    // ---------------------------------------------------------------------------------------------

    int32_t Option::as_int() const
    {
        if (was_found())
            return as_int(_value);

        return as_int(_default_value);
    }

    // ---------------------------------------------------------------------------------------------

    uint32_t Option::as_uint(const std::string &v)
    {
        return static_cast<uint32_t>(std::strtol(v.c_str(), nullptr, 10));
    }

    // ---------------------------------------------------------------------------------------------

    uint32_t Option::as_uint() const
    {
        if (was_found())
            return as_uint(_value);

        return as_uint(_default_value);
    }

    // ---------------------------------------------------------------------------------------------

    double Option::as_double(const std::string &v) { return std::strtod(v.c_str(), nullptr); }

    // ---------------------------------------------------------------------------------------------

    double Option::as_double() const
    {
        if (was_found())
            return as_double(_value);

        return as_double(_default_value);
    }

    // ---------------------------------------------------------------------------------------------

    bool Option::as_bool(const std::string &v)
    {
        return (strcasecmp("true", v.c_str()) == 0) || (as_int(v) != 0);
    }

    // ---------------------------------------------------------------------------------------------

    bool Option::as_bool() const
    {
        if (was_found())
            return as_bool(_value);

        return as_bool(_default_value);
    }

    // ---------------------------------------------------------------------------------------------

    const std::string &Option::as_string() const
    {
        if (_found)
            return _value;

        // not found, so returning default
        return _default_value;
    }
} // namespace Options
