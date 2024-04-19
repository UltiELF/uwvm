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
        inline constexpr ::fast_io::string_view enable_mutable_globals_alias{"-Emtglb"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_mutable_globals{
        .name{::fast_io::string_view{"--enable-mutable-globals"}},
        .describe{::fast_io::u8string_view{
            u8"Enable import or export mutable globals for WebAssembly (https://github.com/WebAssembly/mutable-global/blob/master/proposals/mutable-global/Overview.md). Usage: [--enable-mutable-globals|-Emtglb]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_mutable_globals_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_mutable_globals)},
    };
}  // namespace uwvm::parameter
