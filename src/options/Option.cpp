#include "Option.hpp"

#include <string.h>

namespace Options
{
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
        // if the option was not used - then it is false
        if (!was_found())
            return false;

        // option was used so:
        // in case of argument it must be set to value "true" or non-zero int
        if (has_argument())
            return as_bool(_value);

        // if it was a flag, it was already found, so it is true
        return true;
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
