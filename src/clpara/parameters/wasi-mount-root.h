#pragma once

#include <fast_io.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool wasi_mount_root_is_exist{};
        inline constexpr ::fast_io::string_view wasi_mount_root_alias{"-mr"};
        extern ::uwvm::cmdline::parameter_return_type wasi_mount_root_callback(::uwvm::cmdline::parameter_parsing_results*,
                                                                               ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter wasi_mount_root{
        .name{::fast_io::string_view{"--wasi-mount-root"}},
        .describe{::fast_io::u8string_view{u8"Mount the current folder to the root directory of WASI. Usage: [--wasi-mount-root|-mr] <path>"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::wasi_mount_root_alias), 1}},
        .callback{__builtin_addressof(details::wasi_mount_root_callback)},
        .is_exist{__builtin_addressof(details::wasi_mount_root_is_exist)},
    };
}  // namespace uwvm::parameter
