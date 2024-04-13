#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool enable_multivalue_is_exist{};
        inline constexpr ::fast_io::string_view enable_multivalue_alias{"-Emv"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_multivalue{
        .name{::fast_io::string_view{"--enable-multivalue"}},
        .describe{::fast_io::u8string_view{
            u8"Enable multi-value for WebAssembly (https://github.com/WebAssembly/multi-value). Usage: [--enable-multivalue|-Emv]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_multivalue_alias), 1}},
        .is_exist{__builtin_addressof(details::enable_multivalue_is_exist)},
    };
}  // namespace uwvm::parameter
