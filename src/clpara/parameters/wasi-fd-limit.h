#pragma once

#include <fast_io.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool wasi_fd_limit_is_exist{};
        inline constexpr ::fast_io::string_view wasi_fd_limit_alias{"-Wfl"};
        extern ::uwvm::cmdline::parameter_return_type wasi_fd_limit_callback(::uwvm::cmdline::parameter_parsing_results*,
                                                                               ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter wasi_fd_limit{
        .name{::fast_io::string_view{"--wasi-fd-limit"}},
        .describe{::fast_io::u8string_view{u8"Limit the maximum number of fds available for wasi (1024 by default). Usage: [--wasi-fd-limit|-Wfl] <size_t>"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::wasi_fd_limit_alias), 1}},
        .callback{__builtin_addressof(details::wasi_fd_limit_callback)},
        .is_exist{__builtin_addressof(details::wasi_fd_limit_is_exist)},
    };
}  // namespace uwvm::parameter
