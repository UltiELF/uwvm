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
        inline constexpr ::fast_io::string_view enable_memory64_alias{"-Emem64"};

#if !(defined(__SIZEOF_SIZE_T__) && __SIZEOF_SIZE_T__ == 8)
    #if __has_cpp_attribute(__gnu__::__cold__)
        [[__gnu__::__cold__]]
    #endif
        inline ::uwvm::cmdline::parameter_return_type
            enable_memory64_callback(::uwvm::cmdline::parameter_parsing_results*, ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept
        {
    #if defined(__SIZEOF_SIZE_T__)
            ::fast_io::io::perr(::uwvm::u8err,
                            u8"\033[0m"
        #ifdef __MSDOS__
                            u8"\033[37m"
        #else
                            u8"\033[97m"
        #endif
                            u8"uwvm: "
                            u8"\033[31m"
                            u8"[error] "
        #ifdef __MSDOS__
                            u8"\033[37m"
        #else
                            u8"\033[97m"
        #endif
                            u8"Using a platform that does not support memory64."
                            u8"\033[0m"
                            u8"\n\n");
            return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
    #else
            if constexpr(sizeof(::std::size_t) != 8U)
            {
                ::fast_io::io::perr(::uwvm::u8err,
                            u8"\033[0m"
        #ifdef __MSDOS__
                            u8"\033[37m"
        #else
                            u8"\033[97m"
        #endif
                            u8"uwvm: "
                            u8"\033[31m"
                            u8"[error] "
        #ifdef __MSDOS__
                            u8"\033[37m"
        #else
                            u8"\033[97m"
        #endif
                            u8"Using a platform that does not support memory64."
                            u8"\033[0m"
                            u8"\n\n");
                return ::uwvm::cmdline::parameter_return_type::return_m1_imme;
            }
            return ::uwvm::cmdline::parameter_return_type::def;
    #endif
        }
#endif
    }  // namespace details

    inline constexpr ::uwvm::cmdline::parameter enable_memory64{
        .name{::fast_io::string_view{"--enable-memory64"}},
        .describe{::fast_io::u8string_view{
            u8"Enable 64-bit memory indexes for WebAssembly (https://github.com/WebAssembly/memory64/blob/master/proposals/memory64/Overview.md). Usage: [--enable-memory64|-Emem64]"}},
        .alias{::uwvm::cmdline::kns_str_scatter_t{__builtin_addressof(details::enable_memory64_alias), 1}},
#if !(defined(__SIZEOF_SIZE_T__) && __SIZEOF_SIZE_T__ == 8)
        .callback{__builtin_addressof(details::enable_memory64_callback)},
#endif
        .is_exist{__builtin_addressof(::uwvm::features::enable_memory64)},
    };
}  // namespace uwvm::parameter
