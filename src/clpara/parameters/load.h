#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline constexpr ::fast_io::string_view load_alias{"-l"};
        extern ::uwvm::cmdline::parameter_return_type load_callback(::uwvm::cmdline::parameter_parsing_results*,
                                                                    ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter load{
        .name{::fast_io::string_view{"--load"}},
        .describe{::fast_io::u8string_view{u8"Load plug-in. Usage: [--load|-l] <file>"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::load_alias), 1}},
        .callback{__builtin_addressof(details::load_callback)},
    };
}  // namespace uwvm::parameter
