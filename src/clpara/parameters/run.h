#pragma once

#include <fast_io.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline constexpr ::fast_io::basic_os_c_str_with_known_size<char> run_alias{::fast_io::mnp::os_c_str_arr("-r")};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter run{
        .name{::fast_io::mnp::os_c_str_arr("--run")},
        .describe{::fast_io::mnp::os_c_str_arr(u8"Run WebAssembly. Usage: [--run|-r] <file> <argv1> <argv2> ...")},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::run_alias), 1}},
    };
}  // namespace uwvm::parameter
