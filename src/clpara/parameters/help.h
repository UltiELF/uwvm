#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>

#include <cmdline/parameter.h>

namespace uwvm::parameter
{
    namespace details
    {
        inline bool help_is_exist{};
        inline constexpr ::fast_io::freestanding::array<::fast_io::string_view, 2> help_alias{::fast_io::string_view{"-h"}, ::fast_io::string_view{"-?"}};
        extern ::uwvm::cmdline::parameter_return_type help_callback(::uwvm::cmdline::parameter_parsing_results*,
                                                                    ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept;
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter help{
        .name{::fast_io::string_view{"--help"}},
        .describe{::fast_io::u8string_view{u8"Get help information. Usage: [--help|-h|-?]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{details::help_alias.data(), details::help_alias.size()}},
        .callback{__builtin_addressof(details::help_callback)},
        .is_exist{__builtin_addressof(details::help_is_exist)},
    };
}  // namespace uwvm::parameter
