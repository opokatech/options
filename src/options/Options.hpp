#pragma once

#include "Option.hpp"

namespace Options
{
    class Options
    {
    public:
        Options();
        ~Options();

        // explicitly disallow copying in any form
        Options(const Options &) = delete;
        Options(Options &&)      = delete;
        Options &operator=(const Options &) = delete;
        Options &operator=(Options &&) = delete;

        Option &add(const Option &o);

        bool parse(int argc, const char *const *argv, int start_idx = 1);

        size_t             positional_count() const;
        const std::string &positional(size_t idx);

        int32_t            as_int(const std::string &name) const;
        double             as_double(const std::string &name) const;
        bool               as_bool(const std::string &name) const;
        const std::string &as_string(const std::string &name) const;

        std::string possible_options() const;

    private:
        struct Impl;
        Impl *_impl = nullptr;
    };
} // namespace Options
