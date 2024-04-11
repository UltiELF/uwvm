#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool enable_memory64_is_exist{};
        inline constexpr ::fast_io::string_view enable_memory64_alias{"-Em64"};
        extern ::uwvm::cmdline::parameter_return_type enable_memory64_callback(::uwvm::cmdline::parameter_parsing_results*,
                                                                    ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_memory64{
        .name{::fast_io::string_view{"--enable-memory64"}},
        .describe{::fast_io::u8string_view{u8"Enable 64-bit memory indexes for WebAssembly (https://github.com/WebAssembly/memory64). Usage: [--enable-memory64|-Em64]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_memory64_alias), 1}},
        .callback{__builtin_addressof(details::enable_memory64_callback)},
        .is_exist{__builtin_addressof(details::enable_memory64_is_exist)},
    };
}  // namespace uwvm::parameter
