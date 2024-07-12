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
        inline constexpr ::fast_io::string_view enable_reference_types_alias{"-Eref"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_reference_types{ // default
        .name{::fast_io::string_view{"--enable-reference-types"}},
        .describe{::fast_io::u8string_view{
            u8"Enable type reflection for WebAssembly (https://github.com/WebAssembly/reference-types/blob/master/proposals/reference-types/Overview.md). Usage: [--enable-reference-types|-Eref]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_reference_types_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_reference_types)},
    };
}  // namespace uwvm::parameter
