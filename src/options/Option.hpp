#pragma once

#include <string>

namespace Options
{
    class Option
    {
    public:
        using validator_t = bool (*)(const std::string &);

        Option(const std::string &long_name, char short_name, const std::string &description);

        Option(const std::string &long_name, const std::string &description);

        Option &validator(validator_t v);

        Option &takes_mandatory_argument();

        Option &takes_optional_argument_with_default(const std::string &default_value);

        char               short_name() const { return _short_name; }
        const std::string &long_name() const { return _long_name; }
        const std::string &description() const { return _description; }
        const std::string &default_value() const { return _default_value; }

        bool was_found() const { return _found; }
        bool has_argument() const { return _argument; }
        bool is_mandatory() const { return _mandatory; }
        bool has_default_value() const { return !_default_value.empty(); }

        void found() { _found = true; }
        bool set_value(const std::string &v);

        static int32_t     as_int(const std::string &);
        int32_t            as_int() const;
        static uint32_t    as_uint(const std::string &);
        uint32_t           as_uint() const;
        static double      as_double(const std::string &);
        double             as_double() const;
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
