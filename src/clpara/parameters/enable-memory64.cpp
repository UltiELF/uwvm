#include <cstddef>
#include "enable-memory64.h"

#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]]
#endif
::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::enable_memory64_callback)(
    ::uwvm::cmdline::parameter_parsing_results*,
    ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept
{
#ifdef __SIZEOF_SIZE_T__
    #if __SIZEOF_SIZE_T__ != 8
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
    #endif
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
#endif
    return ::uwvm::cmdline::parameter_return_type::def;
}
