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
        inline constexpr ::fast_io::string_view abi_alias{"-a"};
        extern ::uwvm::cmdline::parameter_return_type abi_callback(::uwvm::cmdline::parameter_parsing_results*,
                                                                   ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter abi{
        .name{::fast_io::string_view{"--abi"}},
        .describe{::fast_io::u8string_view{u8"Specifies the ABI used by the WASM module. Usage: [--abi|-a] [bare|emscripten|wasi]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::abi_alias), 1}},
        .callback{__builtin_addressof(details::abi_callback)},
        .is_exist{__builtin_addressof(details::abi_is_exist)},
    };
}  // namespace uwvm::parameter
