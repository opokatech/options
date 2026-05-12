#pragma once

#include <cstdint>
#include <string>

namespace Options
{
    int32_t as_int(const std::string &value);
    uint32_t as_uint(const std::string &value);
    double as_double(const std::string &value);
    bool as_bool(const std::string &value);

} // namespace Options
