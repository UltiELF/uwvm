#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool enable_multi_value_is_exist{};
        inline constexpr ::fast_io::string_view enable_multi_value_alias{"-Emval"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_multi_value{
        .name{::fast_io::string_view{"--enable-multi-value"}},
        .describe{
            ::fast_io::u8string_view{u8"Enable multi-value for WebAssembly (https://github.com/WebAssembly/multi-value). Usage: [--enable-multi-value|-Emval]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_multi_value_alias), 1}},
        .is_exist{__builtin_addressof(details::enable_multi_value_is_exist)},
    };
}  // namespace uwvm::parameter
