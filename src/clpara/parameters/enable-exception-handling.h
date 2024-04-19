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
        inline constexpr ::fast_io::string_view enable_exception_handling_alias{"-Eeh"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_exception_handling{
        .name{::fast_io::string_view{"--enable-exception-handling"}},
        .describe{::fast_io::u8string_view{
            u8"Enable exception handling for WebAssembly (https://github.com/WebAssembly/exception-handling/blob/master/proposals/exception-handling/Exceptions.md). Usage: [--enable-exception-handling|-Eeh]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_exception_handling_alias), 1}},
        .is_exist{__builtin_addressof(::uwvm::features::enable_exception_handling)},
    };
}  // namespace uwvm::parameter
