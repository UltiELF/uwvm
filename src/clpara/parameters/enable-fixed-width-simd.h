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
        inline constexpr ::fast_io::string_view enable_fixed_width_simd_alias{"-Esimd"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_fixed_width_simd{
        .name{::fast_io::string_view{"--enable-fixed-width-simd"}},
        .describe{::fast_io::u8string_view{
            u8"Enable fixed-width simd for WebAssembly (https://github.com/WebAssembly/simd/blob/master/proposals/simd/SIMD.md). Usage: [--enable-fixed-width-simd|-Esimd]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_fixed_width_simd_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_fixed_width_simd)},
    };
}  // namespace uwvm::parameter
