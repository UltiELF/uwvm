#pragma once

#include <fast_io.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool version_is_exist{};
        inline constexpr ::fast_io::string_view version_alias{::fast_io::mnp::os_c_str_arr("-v")};
        extern ::uwvm::cmdline::parameter_return_type version_callback(::uwvm::cmdline::parameter_parsing_results*,
                                                                       ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter version{
        .name{::fast_io::mnp::os_c_str_arr("--version")},
        .describe{::fast_io::mnp::os_c_str_arr(u8"Output version information. Usage: [--version|-v]")},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::version_alias), 1}},
        .callback{__builtin_addressof(details::version_callback)},
        .is_exist{__builtin_addressof(details::version_is_exist)},
    };
}  // namespace uwvm::parameter
