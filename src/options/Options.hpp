#pragma once

#include <vector>

#include "Option.hpp"

namespace Options
{
    class Options
    {
    public:
        Option &add(const Option &o);

        bool parse(int argc, const char *const *argv, int start_idx = 1);

        size_t             positional_count() const { return _positional.size(); }
        const std::string &positional(size_t idx) { return _positional.at(idx); }

        int32_t            as_int(const std::string &name) const;
        double             as_double(const std::string &name) const;
        bool               as_bool(const std::string &name) const;
        const std::string &as_string(const std::string &name) const;

        std::string possible_options() const;

    private:
        using opts_t = std::vector<Option>;
        using pos_t  = std::vector<std::string>;

        opts_t::const_iterator find_option_by_long_name(const std::string &) const;
        opts_t::iterator       find_option_by_name_with_dashes(const std::string &);

        opts_t                   _options;
        uint32_t                 _longest_option_name = 0;
        std::vector<std::string> _positional;
    };
} // namespace Options
