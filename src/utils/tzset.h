#pragma once

#include <fast_io.h>

namespace uwvm
{
    struct tz_set_s
    {
        tz_set_s() noexcept { ::fast_io::posix_tzset(); }
    };
}  // namespace uwvm
