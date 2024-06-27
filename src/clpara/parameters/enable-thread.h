#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>
#include <cmdline/parameter.h>
#include "../../run/features.h"

namespace uwvm::parameter
{
    namespace details
    {
        inline constexpr ::fast_io::string_view enable_thread_alias{"-Eth"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_thread{
        .name{::fast_io::string_view{"--enable-thread"}},
        .describe{::fast_io::u8string_view{
            u8"Enable threads and atomics for WebAssembly (https://github.com/WebAssembly/threads/blob/master/proposals/threads/Overview.md). Usage: [--enable-thread|-Eth]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_thread_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_thread)},
    };
}  // namespace uwvm::parameter
