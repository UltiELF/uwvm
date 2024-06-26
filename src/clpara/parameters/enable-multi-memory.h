﻿#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>
#include <cmdline/parameter.h>
#include "../../run/features.h"

namespace uwvm::parameter
{
    namespace details
    {
        inline constexpr ::fast_io::string_view enable_multi_memory_alias{"-Emmem"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_multi_memory{
        .name{::fast_io::string_view{"--enable-multi-memory"}},
        .describe{
            ::fast_io::u8string_view{u8"Enable multi memory for WebAssembly (https://github.com/WebAssembly/multi-memory/blob/master/proposals/multi-memory/Overview.md). Usage: [--enable-multi-memory|-Emmem]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_multi_memory_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_multi_memory)},
    };
}  // namespace uwvm::parameter
