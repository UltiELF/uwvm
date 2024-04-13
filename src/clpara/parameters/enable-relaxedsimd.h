#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool enable_relaxedsimd_is_exist{};
        inline constexpr ::fast_io::string_view enable_relaxedsimd_alias{"-Esimd"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_relaxedsimd{
        .name{::fast_io::string_view{"--enable-relaxedsimd"}},
        .describe{
            ::fast_io::u8string_view{u8"Enable relaxed simd for WebAssembly (https://github.com/WebAssembly/relaxed-simd). Usage: [--enable-relaxedsimd|-Esimd]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_relaxedsimd_alias), 1}},
        .is_exist{__builtin_addressof(details::enable_relaxedsimd_is_exist)},
    };
}  // namespace uwvm::parameter
