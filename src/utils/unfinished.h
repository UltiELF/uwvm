#pragma once
#include <source_location>
#include <fast_io.h>
#include "io_device.h"

namespace uwvm
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    [[noreturn]] inline constexpr void unfinished(::std::source_location const& location = ::std::source_location::current()) noexcept
    {
        ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
#ifdef __MSDOS__
                                u8"\033[34m"
#else
                                u8"\033[94m"
#endif
                                u8"[debug] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"Unfinished: ",
                                location,
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");

        ::fast_io::fast_terminate();
    }
}  // namespace uwvm
