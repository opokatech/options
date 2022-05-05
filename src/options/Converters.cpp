#include <string.h> // strcasecmp

#include "Converters.hpp"

namespace Options
{
    int32_t as_int(const std::string &v)
    {
        return static_cast<int32_t>(std::strtol(v.c_str(), nullptr, 10));
    }

    // ---------------------------------------------------------------------------------------------

    uint32_t as_uint(const std::string &v)
    {
        return static_cast<uint32_t>(std::strtol(v.c_str(), nullptr, 10));
    }

    // ---------------------------------------------------------------------------------------------

    double as_double(const std::string &v) { return std::strtod(v.c_str(), nullptr); }

    // ---------------------------------------------------------------------------------------------

    bool as_bool(const std::string &v)
    {
        return (strcasecmp("true", v.c_str()) == 0) || (as_int(v) != 0);
    }

} // namespace Options
