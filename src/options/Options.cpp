#include <algorithm>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

#include "Option.hpp"
#include "Options.hpp"

namespace Options
{
    struct Options::Impl
    {
        // This will throw an exception if the option is not found.
        std::vector<Option>::const_iterator find_option_by_long_name(const std::string &name) const
        {
            auto iter = std::find_if(_options.cbegin(), _options.cend(),
                                     [&name](const Option &opt) { return opt.long_name() == name; });

            if (iter == _options.cend())
                throw std::logic_error("option '" + name + "' not found");

            return iter;
        }

        // Return an iterator to the option if found, or _options.end() otherwise.
        std::vector<Option>::iterator find_option_by_name_with_dashes(const std::string &name)
        {
            return std::find_if(_options.begin(), _options.end(), [&name](const Option &opt) {
                if (name == (std::string("-") + opt.short_name()))
                    return true;

                if (name == ("--" + opt.long_name()))
                    return true;

                return false;
            });
        }

        Option &add(const Option &&opt)
        {
            _options.emplace_back(opt);

            if (opt.long_name().size() > _longest_option_name)
                _longest_option_name = opt.long_name().size();

            return _options.back();
        }

        std::vector<Option> _options;
        uint32_t _longest_option_name = 0;
        std::vector<std::string> _positional;
    };

    Options::Options() : _impl(new Impl) {}

    Options::~Options()
    {
        delete _impl;
    }

    void Options::add_flag(const std::string &long_name, char short_name, const std::string &description)
    {
        _impl->add({long_name, short_name, description});
    }

    void Options::add_flag(const std::string &long_name, const std::string &description)
    {
        add_flag(long_name, Option::SHORT_NOT_USED, description);
    }

    void Options::add_optional(const std::string &long_name, char short_name, const std::string &description,
                               const std::string &default_value, validator_t validator)
    {
        _impl->add({long_name, short_name, description}).set_optional(default_value).set_validator(validator);
    }

    void Options::add_optional(const std::string &long_name, const std::string &description,
                               const std::string &default_value, validator_t validator)
    {
        add_optional(long_name, Option::SHORT_NOT_USED, description, default_value, validator);
    }

    void Options::add_mandatory(const std::string &long_name, char short_name, const std::string &description,
                                validator_t validator)
    {
        _impl->add({long_name, short_name, description}).set_mandatory().set_validator(validator);
    }

    void Options::add_mandatory(const std::string &long_name, const std::string &description, validator_t validator)
    {
        add_mandatory(long_name, Option::SHORT_NOT_USED, description, validator);
    }

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
                    auto iter = _impl->find_option_by_name_with_dashes(argv[pos]);

                    if (iter == _impl->_options.end()) // not found
                        return false;

                    if (iter->has_argument())
                    {
                        pos += 1;
                        if (pos >= argc) // value not found
                            return false;

                        // set the value and validate it if there is a validator
                        if (!iter->set_value(argv[pos]))
                            return false;
                    }
                    else // must be a flag
                    {
                        if (!iter->set_value("true"))
                            return false;
                    }
                }
            }

            pos += 1;
        }

        // this method succeeds if all the mandatory options were found and set
        const auto NON_MANDATORY_OR_FOUND = [](const Option &opt) { return (!opt.is_mandatory() || opt.was_set()); };

        return std::all_of(_impl->_options.cbegin(), _impl->_options.cend(), NON_MANDATORY_OR_FOUND);
    }

    size_t Options::positional_count() const
    {
        return _impl->_positional.size();
    }

    const std::string &Options::positional(size_t idx) const
    {
        return _impl->_positional.at(idx);
    }

    int32_t Options::as_int(const std::string &name) const
    {
        return _impl->find_option_by_long_name(name)->as_int();
    }

    uint32_t Options::as_uint(const std::string &name) const
    {
        return _impl->find_option_by_long_name(name)->as_uint();
    }

    double Options::as_double(const std::string &name) const
    {
        return _impl->find_option_by_long_name(name)->as_double();
    }

    bool Options::as_bool(const std::string &name) const
    {
        return _impl->find_option_by_long_name(name)->as_bool();
    }

    const std::string &Options::as_string(const std::string &name) const
    {
        return _impl->find_option_by_long_name(name)->as_string();
    }

    std::string Options::get_possible_options() const
    {
        std::stringstream sstream;

        std::left(sstream); // align left

        constexpr int32_t MIN_TEXT_WIDTH = 8;
        const int32_t SPACE_FOR_NAMES = MIN_TEXT_WIDTH + static_cast<int32_t>(_impl->_longest_option_name);

        for (const auto &opt: _impl->_options)
        {
            std::string short_long;

            if (opt.short_name() != Option::SHORT_NOT_USED)
                short_long += std::string("-") + opt.short_name() + ", ";
            else
                short_long += "    ";

            short_long += ("--" + opt.long_name());

            sstream << " " << std::setw(SPACE_FOR_NAMES) << short_long << (opt.is_mandatory() ? "M " : "  ");

            sstream << opt.description();

            if (opt.is_optional())
                sstream << " (default: " << opt.default_value() + ")";

            sstream << std::endl;
        }

        return sstream.str();
    }
} // namespace Options
