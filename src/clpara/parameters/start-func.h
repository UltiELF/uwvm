#pragma once

#include <fast_io.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool start_func_is_exist{};
        inline constexpr ::fast_io::string_view start_func_alias{"-sf"};
        extern ::uwvm::cmdline::parameter_return_type start_func_callback(::uwvm::cmdline::parameter_parsing_results*,
                                                                          ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;

    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter start_func{
        .name{::fast_io::string_view{"--start-func"}},
        .describe{::fast_io::u8string_view{u8"Set the startup function for wasm. Usage: [--start-func|-sf] <size_t>"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::start_func_alias), 1}},
        .callback{__builtin_addressof(details::start_func_callback)},
        .is_exist{__builtin_addressof(details::start_func_is_exist)},
    };
}  // namespace uwvm::parameter
