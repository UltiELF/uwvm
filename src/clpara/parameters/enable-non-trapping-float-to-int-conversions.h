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
        inline constexpr ::fast_io::string_view enable_non_trapping_float_to_int_conversion_alias{"-Entfticv"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_non_trapping_float_to_int_conversion{
        // default
        .name{::fast_io::string_view{"--enable-non-trapping-fti-conversion"}},
        .describe{::fast_io::u8string_view{
            u8"Enable Non-trapping float-to-int conversions for WebAssembly (https://github.com/WebAssembly/spec/blob/master/proposals/nontrapping-float-to-int-conversion/Overview.md). Usage: [--enable-non-trapping-fti-conversion|-Entfticv]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_non_trapping_float_to_int_conversion_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_non_trapping_float_to_int_conversion)},
    };
}  // namespace uwvm::parameter
