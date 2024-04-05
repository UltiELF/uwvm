#include "help.h"
#include "../parameters.h"

#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]]
#endif
::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::help_callback)(::std::size_t,
                                                                                  ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept
{
    ::fast_io::u8native_io_observer buf_u8out{::uwvm::u8err};
    ::fast_io::io::perr(buf_u8out,
                        u8"\033[0m"
#ifdef __MSDOS__
                        u8"\033[37m"
#else
                        u8"\033[97m"
#endif
                        u8"Arguments:\n");
    for(auto& p: ::uwvm::parameters)
    {
        ::fast_io::io::perr(buf_u8out, u8"\t" u8"\033[32m", ::fast_io::mnp::code_cvt(p->name));
        if(p->alias.len != 0)
        {
            ::fast_io::io::perr(buf_u8out,
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8", ");
            for(::std::size_t i{}; i < p->alias.len - 1; i++)
            {
                ::fast_io::io::perr(buf_u8out,
                                    u8"\033[32m",
                                    ::fast_io::mnp::code_cvt(p->alias.base[i]),
#ifdef __MSDOS__
                                    u8"\033[37m"
#else
                                    u8"\033[97m"
#endif
                                    u8", ");
            }
            ::fast_io::io::perr(buf_u8out, u8"\033[32m", ::fast_io::mnp::code_cvt(p->alias.base[p->alias.len - 1]));
        }
        ::fast_io::io::perrln(buf_u8out,
                               u8"\033[37m"
                               u8" --- "
#ifdef __MSDOS__
                               u8"\033[37m"
#else
                               u8"\033[97m"
#endif
                               ,
                               p->describe);
    }
    ::fast_io::io::perr(buf_u8out, u8"\033[0m" u8"\n");

    return ::uwvm::cmdline::parameter_return_type::return_imme;
}
