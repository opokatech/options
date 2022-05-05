#pragma once

#include <string>

namespace Options
{
    using validator_t = bool (*)(const std::string &);
}
