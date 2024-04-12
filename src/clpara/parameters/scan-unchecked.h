#pragma once

#include <fast_io.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool scan_unchecked_is_exist{};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter scan_unchecked{
        .name{::fast_io::string_view{"--scan-unchecked"}},
        .describe{::fast_io::u8string_view{u8"Disable all checks when scanning wasm files. Usage: [--scan-unchecked]"}},
        .is_exist{__builtin_addressof(details::scan_unchecked_is_exist)},
    };
}  // namespace uwvm::parameter
