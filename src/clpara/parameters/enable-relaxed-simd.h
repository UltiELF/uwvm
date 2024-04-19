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
        inline constexpr ::fast_io::string_view enable_relaxed_simd_alias{"-Erlxsimd"};
        extern ::uwvm::cmdline::parameter_return_type enable_relaxed_simd_callback(::uwvm::cmdline::parameter_parsing_results*,
                                                                                   ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;

    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_relaxed_simd{
        .name{::fast_io::string_view{"--enable-relaxed-simd"}},
        .describe{::fast_io::u8string_view{
            u8"Enable relaxed simd for WebAssembly (https://github.com/WebAssembly/relaxed-simd/tree/main/proposals/relaxed-simd/Overview.md). Usage: [--enable-relaxed-simd|-Erlxsimd]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_relaxed_simd_alias), 1}},
        .callback{__builtin_addressof(details::enable_relaxed_simd_callback)},
        .is_exist{__builtin_addressof(::uwvm::features::enable_relaxed_simd)},
    };
}  // namespace uwvm::parameter
