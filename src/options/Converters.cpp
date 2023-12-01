#include <string.h> // strcasecmp

#include "Converters.hpp"

namespace Options
{
    int32_t as_int(const std::string &value)
    {
        constexpr int32_t DEC_BASE = 10;
        return static_cast<int32_t>(std::strtol(value.c_str(), nullptr, DEC_BASE));
    }

    uint32_t as_uint(const std::string &value)
    {
        constexpr int32_t DEC_BASE = 10;
        return static_cast<uint32_t>(std::strtol(value.c_str(), nullptr, DEC_BASE));
    }

    double as_double(const std::string &value)
    {
        return std::strtod(value.c_str(), nullptr);
    }

    bool as_bool(const std::string &value)
    {
        return (strcasecmp("true", value.c_str()) == 0) || (as_int(value) != 0);
    }
} // namespace Options
