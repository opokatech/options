#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <vector>

#include "Options.hpp"

namespace Options
{
    struct Options::Impl
    {
        using opts_t = std::vector<Option>;
        using pos_t  = std::vector<std::string>;

        opts_t::const_iterator find_option_by_long_name(const std::string &name) const
        {
            auto o = std::find_if(_options.cbegin(), _options.cend(),
                                  [&name](const Option &o) { return o.long_name() == name; });

            if (o == _options.cend())
                throw std::logic_error("option '" + name + "' not found");

            return o;
        }

        opts_t::iterator find_option_by_name_with_dashes(const std::string &name)
        {
            return std::find_if(_options.begin(), _options.end(), [&name](const Option &o) {
                if (name == (std::string("-") + o.short_name()))
                    return true;

                if (name == ("--" + o.long_name()))
                    return true;

                return false;
            });
        }

        opts_t                   _options;
        uint32_t                 _longest_option_name = 0;
        std::vector<std::string> _positional;
    };

    // ---------------------------------------------------------------------------------------------

    Options::Options() : _impl(new Impl) {}

    // ---------------------------------------------------------------------------------------------

    Options::~Options() { delete _impl; }

    // ---------------------------------------------------------------------------------------------

    Option &Options::add(const Option &o)
    {
        _impl->_options.push_back(o);

        if (o.long_name().size() > _impl->_longest_option_name)
            _impl->_longest_option_name = o.long_name().size();

        return _impl->_options.back();
    }

    // ---------------------------------------------------------------------------------------------

    bool Options::parse(int argc, const char *const *argv, int start_idx)
    {
        int pos = start_idx;

        bool collect_positionals = false;

        while (pos < argc)
        {
            if (collect_positionals)
                _impl->_positional.push_back(argv[pos]);
            else
            {
                if (strcmp(argv[pos], "--") == 0)
                    collect_positionals = true;
                else
                {
                    auto o = _impl->find_option_by_name_with_dashes(argv[pos]);

                    if (o == _impl->_options.end()) // not found
                        return false;

                    if (o->has_argument())
                    {
                        pos += 1;
                        if (pos >= argc) // value not found
                            return false;

                        // set the value and validate it if there is a validator
                        if (!o->set_value(argv[pos]))
                            return false;
                    }
                    else // must be a flag
                    {
                        if (!o->set_value("true"))
                            return false;
                    }
                }
            }

            pos += 1;
        }

        // this method succeeds if we all mandatory options were found
        auto non_mandatory_or_found = [](const Option &o) {
            return (!o.is_mandatory() || o.was_found());
        };

        return std::all_of(_impl->_options.cbegin(), _impl->_options.cend(),
                           non_mandatory_or_found);
    }

    // ---------------------------------------------------------------------------------------------

    size_t Options::positional_count() const { return _impl->_positional.size(); }

    // ---------------------------------------------------------------------------------------------

    const std::string &Options::positional(size_t idx) { return _impl->_positional.at(idx); }

    // ---------------------------------------------------------------------------------------------

    int32_t Options::as_int(const std::string &name) const
    {
        return _impl->find_option_by_long_name(name)->as_int();
    }

    // ---------------------------------------------------------------------------------------------

    double Options::as_double(const std::string &name) const
    {
        return _impl->find_option_by_long_name(name)->as_double();
    }

    // ---------------------------------------------------------------------------------------------

    bool Options::as_bool(const std::string &name) const
    {
        return _impl->find_option_by_long_name(name)->as_bool();
    }

    // ---------------------------------------------------------------------------------------------

    const std::string &Options::as_string(const std::string &name) const
    {
        return _impl->find_option_by_long_name(name)->as_string();
    }

    // ---------------------------------------------------------------------------------------------

    std::string Options::possible_options() const
    {
        std::stringstream ss;

        std::left(ss); // align left

        const int32_t SPACE_FOR_NAMES = 8 + static_cast<int32_t>(_impl->_longest_option_name);

        for (const auto &o: _impl->_options)
        {
            std::string short_long;

            if (o.short_name() != Option::SHORT_NOT_USED)
                short_long += std::string("-") + o.short_name() + ", ";
            else
                short_long += "    ";

            short_long += ("--" + o.long_name());

            ss << " " << std::setw(SPACE_FOR_NAMES) << short_long
               << (o.is_mandatory() ? "M " : "  ");

            ss << o.description();

            if (o.has_default_value())
                ss << " (default: " << o.default_value() + ")";

            ss << std::endl;
        }

        return ss.str();
    }
} // namespace Options
