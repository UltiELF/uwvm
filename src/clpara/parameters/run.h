#pragma once

#include <fast_io.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline constexpr ::fast_io::string_view run_alias{"-r"};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter run{
        .name{::fast_io::string_view{"--run"}},
        .describe{::fast_io::u8string_view{u8"Run WebAssembly. Usage: [--run|-r] <file> <argv1> <argv2> ..."}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::run_alias), 1}},
    };
}  // namespace uwvm::parameter
