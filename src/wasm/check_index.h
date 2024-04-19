#pragma once
#include <fast_io.h>
#include <io_device.h>
#include "../run/features.h"

namespace uwvm::wasm
{
    inline void check_index(::std::size_t s) noexcept
    {
#if defined(__SIZEOF_SIZE_T__) && __SIZEOF_SIZE_T__ == 8
        auto const has_enable_memory64_alias{::uwvm::features::enable_memory64};
        constexpr auto u32max{static_cast<::std::size_t>(::std::numeric_limits<::std::uint_least32_t>::max())};
        if(!has_enable_memory64_alias && s > u32max) [[unlikely]]
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
                                u8"[fatal] "
                                u8"\033[0m"
    #ifdef __MSDOS__
                                u8"\033[37m"
    #else
                                u8"\033[97m"
    #endif
                                u8"64-bit memory addresses requires memory64 feature."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
#else
        if constexpr(sizeof(::std::size_t) == 8)
        {
            auto const has_enable_memory64_alias{::uwvm::features::enable_memory64};
            constexpr auto u32max{static_cast<::std::size_t>(::std::numeric_limits<::std::uint_least32_t>::max())};
            if(!has_enable_memory64_alias && s > u32max) [[unlikely]]
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
                                u8"[fatal] "
                                u8"\033[0m"
    #ifdef __MSDOS__
                                u8"\033[37m"
    #else
                                u8"\033[97m"
    #endif
                                u8"64-bit memory addresses requires memory64 feature."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }
        }
#endif
    }

}  // namespace uwvm
