#pragma once

#include <string>

namespace Options
{
    class Option
    {
    public:
        using validator_t = bool (*)(const std::string &);

        Option(const std::string &long_name, char short_name, const std::string &description)
            : _short_name{short_name}, _long_name{long_name}, _description{description}
        {
        }

        Option(const std::string &long_name, const std::string &description) // NOLINT
            : Option(long_name, SHORT_NOT_USED, description)
        {
        }

        Option &default_value(const std::string &default_value)
        {
            _default_value = default_value;
            return *this;
        }

        Option &validator(validator_t v)
        {
            _validator = v;
            return *this;
        }

        Option &argument()
        {
            _argument = true;
            return *this;
        }

        Option &mandatory()
        {
            _mandatory = true;
            return *this;
        }

        char               short_name() const { return _short_name; }
        const std::string &long_name() const { return _long_name; }
        const std::string &description() const { return _description; }
        const std::string &default_value() const { return _default_value; }

        void found() { _found = true; }
        bool was_found() const { return _found; }

        bool has_argument() const { return _argument; }
        bool is_mandatory() const { return _mandatory; }
        bool has_default_value() const { return !_default_value.empty(); }

        bool set_value(const std::string &v)
        {
            if (_validator != nullptr)
                if (!_validator(v))
                    return false;

            _found = true;
            _value = v;
            return true;
        }

        static int32_t     as_int(const std::string &);
        int32_t            as_int() const { return as_int(_value); }
        static double      as_double(const std::string &);
        double             as_double() const { return as_double(_value); }
        static bool        as_bool(const std::string &);
        bool               as_bool() const;
        const std::string &as_string() const;

        static constexpr char SHORT_NOT_USED = 0;

    private:
        char        _short_name = SHORT_NOT_USED;
        std::string _long_name;
        std::string _description;

        bool        _argument  = false;
        bool        _mandatory = false;
        std::string _default_value;
        validator_t _validator = nullptr;

        bool        _found = false;
        std::string _value;
    };
} // namespace Options
