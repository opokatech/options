#pragma once

#include <cstdint>
#include <string>

#include "Validator.hpp"

namespace Options
{
    /* Class Options.
     *
     * This class defines expected and possible options passed to the program.
     *
     * There are 3 types of options:
     * - flag - which takes no arguments and can be set or not,
     * - optional - which takes an argument, but only optionally. If not provided a defined default
     *   is used,
     * - mandatory - which takes an argument which must be provided.
     *
     * Every option is defined by a single long name and optional short name. A long name like
     * "mode" must be given to the program with double dashes like so: "--mode".
     * A short name is a single character and must be given with a single dash (e.g. short option
     * 'v' should be given as "-v").
     *
     * Arguments to an option are expected after an empty character like so: "--mode something".
     *
     * Optional and mandatory options may have a validator, which simply returns true if a value
     * that suppose to be used is correct.
     *
     * When object of this class during parsing parameters encounters standalone "--" then it stops
     * looking for defined parameters and treat everything after that as positional arguments. They
     * can be accessed via the api below.
     *
     * Extensive example of how to use this class is in example/example.cpp.
     */
    class Options
    {
    public:
        Options();
        ~Options();

        // explicitly disallow copying in any form
        Options(const Options &) = delete;
        Options(Options &&) = delete;
        Options &operator=(const Options &) = delete;
        Options &operator=(Options &&) = delete;

        void add_flag(const std::string &long_name, char short_name, const std::string &description);

        void add_flag(const std::string &long_name, const std::string &description);

        void add_optional(const std::string &long_name, char short_name, const std::string &description,
                          const std::string &default_value, validator_t validator = nullptr);

        void add_optional(const std::string &long_name, const std::string &description,
                          const std::string &default_value, validator_t validator = nullptr);

        void add_mandatory(const std::string &long_name, char short_name, const std::string &description,
                           validator_t validator = nullptr);

        void add_mandatory(const std::string &long_name, const std::string &description,
                           validator_t validator = nullptr);

        bool parse(int argc, const char *const *argv, int start_idx = 1);

        size_t positional_count() const;
        const std::string &positional(size_t idx) const;

        int32_t as_int(const std::string &name) const;
        uint32_t as_uint(const std::string &name) const;
        double as_double(const std::string &name) const;
        bool as_bool(const std::string &name) const;
        const std::string &as_string(const std::string &name) const;

        std::string get_possible_options() const;

    private:
        struct Impl;
        Impl *_impl = nullptr;
    };
} // namespace Options
