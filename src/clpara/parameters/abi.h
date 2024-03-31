#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool abi_is_exist{};
        constexpr inline ::fast_io::basic_os_c_str_with_known_size<char> abi_alias{::fast_io::mnp::os_c_str_arr("-a")};
        extern ::uwvm::cmdline::parameter_return_type abi_callback(::std::size_t, ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;
    }  // namespace details

    constexpr inline ::uwvm::cmdline::parameter abi{
        .name{::fast_io::mnp::os_c_str_arr("--abi")},
        .describe{::fast_io::mnp::os_c_str_arr(u8"Specifies the ABI used by the WASM module. Usage: [--abi|-a] [bare|emscripten|wasi]")},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::abi_alias), 1}},
        .callback{__builtin_addressof(details::abi_callback)},
        .is_exist{__builtin_addressof(details::abi_is_exist)},
    };
}  // namespace uwvm::parameter
