#include "Option.hpp"
#include "Converters.hpp"

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

    Option &Option::set_mandatory()
    {
        _type = Type::Mandatory;
        return *this;
    }

    // ---------------------------------------------------------------------------------------------

    Option &Option::set_optional(const std::string &default_value)
    {
        _type          = Type::Optional;
        _default_value = default_value;
        return *this;
    }

    // ---------------------------------------------------------------------------------------------

    Option &Option::set_validator(validator_t v)
    {
        _validator = v;
        return *this;
    }

    // ---------------------------------------------------------------------------------------------

    bool Option::set_value(const std::string &v)
    {
        if (has_argument() && _validator != nullptr)
            if (!_validator(v))
                return false;

        _was_set = true;
        _value   = v;
        return true;
    }

    // ---------------------------------------------------------------------------------------------

    int32_t Option::as_int() const
    {
        if (was_set())
            return Options::as_int(_value);

        return Options::as_int(_default_value);
    }

    // ---------------------------------------------------------------------------------------------

    uint32_t Option::as_uint() const
    {
        if (was_set())
            return Options::as_uint(_value);

        return Options::as_uint(_default_value);
    }

    // ---------------------------------------------------------------------------------------------

    double Option::as_double() const
    {
        if (was_set())
            return Options::as_double(_value);

        return Options::as_double(_default_value);
    }

    // ---------------------------------------------------------------------------------------------

    bool Option::as_bool() const
    {
        if (was_set())
            return Options::as_bool(_value);

        return Options::as_bool(_default_value);
    }

    // ---------------------------------------------------------------------------------------------

    const std::string &Option::as_string() const
    {
        if (was_set())
            return _value;

        return _default_value;
    }
} // namespace Options
