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
        inline constexpr ::fast_io::string_view enable_sign_extension_ops_alias{"-Eseops"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_sign_extension_ops{ // default
        .name{::fast_io::string_view{"--enable-sign-extension-ops"}},
        .describe{::fast_io::u8string_view{
            u8"Enable sign extension operators for WebAssembly (https://github.com/WebAssembly/sign-extension-ops/blob/master/proposals/sign-extension-ops/Overview.md). Usage: [--enable-sign-extension-ops|-Eseops]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_sign_extension_ops_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_sign_extension_ops)},
    };
}  // namespace uwvm::parameter
