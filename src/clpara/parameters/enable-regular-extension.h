#pragma once

#include <fast_io.h>
#include <fast_io_freestanding.h>

#include <io_device.h>
#include <cmdline/parameter.h>
#include "../../run/features.h"

namespace uwvm::parameter
{
    namespace details
    {
        inline constexpr ::fast_io::string_view enable_regular_extension_alias{"-ER"};
#if __has_cpp_attribute(__gnu__::__cold__)
        [[__gnu__::__cold__]]
#endif
        inline ::uwvm::cmdline::parameter_return_type
            enable_regular_extension_callback(::uwvm::cmdline::parameter_parsing_results*, ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept
        {
            ::uwvm::features::enable_mutable_globals = true;
            ::uwvm::features::enable_reference_types = true;
            ::uwvm::features::enable_sign_extension_ops = true;
            ::uwvm::features::enable_bulk_memory_operations = true;
            ::uwvm::features::enable_non_trapping_float_to_int_conversion = true;

            return ::uwvm::cmdline::parameter_return_type::def;
        }
        inline bool enable_regular_extension_is_exist{};
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_regular_extension{
        .name{::fast_io::string_view{"--enable-regular-extension"}},
        .describe{::fast_io::u8string_view{
            u8"Equivalent to \"--enable-mutable-globals --enable-reference-types --enable-sign-extension-ops --enable-bulk-memory-operations --enable-non-trapping-fti-conversion\" . Usage: [--enable-regular-extension|-ER]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_regular_extension_alias), 1}},
        .callback{__builtin_addressof(details::enable_regular_extension_callback)},
        .is_exist{__builtin_addressof(details::enable_regular_extension_is_exist)},
    };
}  // namespace uwvm::parameter
