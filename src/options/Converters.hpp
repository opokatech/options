#pragma once

#include <cstdint>
#include <string>

namespace Options
{
    int32_t  as_int(const std::string &);
    uint32_t as_uint(const std::string &);
    double   as_double(const std::string &);
    bool     as_bool(const std::string &);

} // namespace Options
