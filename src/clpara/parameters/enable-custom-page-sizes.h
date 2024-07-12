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
        inline constexpr ::fast_io::string_view enable_custom_page_sizes_alias{"-Ecpsz"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_custom_page_sizes{
        .name{::fast_io::string_view{"--enable-custom-page-sizes"}},
        .describe{::fast_io::u8string_view{
            u8"Enable custom page sizes for WebAssembly (https://github.com/WebAssembly/custom-page-sizes/blob/main/proposals/custom-page-sizes/Overview.md). Usage: [--enable-custom-page-sizes|-Ecpsz]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_custom_page_sizes_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_custom_page_sizes)},
    };
}  // namespace uwvm::parameter
