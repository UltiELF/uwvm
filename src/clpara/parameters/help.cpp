#include "help.h"
#include "../parameters.h"

namespace uwvm::parameter::details
{
    struct help_printer
    {
    };

    template <typename s>
    inline constexpr void print_define(::fast_io::io_reserve_type_t<char8_t, help_printer>, s&& stm, help_printer) noexcept
    {
        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                         u8"\033[0m"
#ifdef __MSDOS__
                                                         u8"\033[37m"
#else
                                                         u8"\033[97m"
#endif
                                                         u8"Arguments:\n");
        for(auto const& p: ::uwvm::parameters)
        {
            ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8"\t" u8"\033[32m", ::fast_io::mnp::code_cvt(p->name));
            if(p->alias.len != 0)
            {
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
#ifdef __MSDOS__
                                                                 u8"\033[37m"
#else
                                                                 u8"\033[97m"
#endif
                                                                 u8", ");
                for(::std::size_t i{}; i < p->alias.len - 1; i++)
                {
                    ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                     u8"\033[32m",
                                                                     ::fast_io::mnp::code_cvt(p->alias.base[i]),
#ifdef __MSDOS__
                                                                     u8"\033[37m"
#else
                                                                     u8"\033[97m"
#endif
                                                                     u8", ");
                }
                ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm),
                                                                 u8"\033[32m",
                                                                 ::fast_io::mnp::code_cvt(p->alias.base[p->alias.len - 1]));
            }
            ::fast_io::operations::print_freestanding<true>(::std::forward<s>(stm),
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
        ::fast_io::operations::print_freestanding<false>(::std::forward<s>(stm), u8"\033[0m" u8"\n");
    }
}  // namespace uwvm::parameter::details

#if __has_cpp_attribute(__gnu__::__cold__)
[[__gnu__::__cold__]]
#endif
::uwvm::cmdline::parameter_return_type(::uwvm::parameter::details::help_callback)(::uwvm::cmdline::parameter_parsing_results*,
                                                                                  ::fast_io::vector<::uwvm::cmdline::parameter_parsing_results>&) noexcept
{
    ::fast_io::basic_obuf<decltype(::uwvm::u8out)> u8outbuf{::uwvm::u8out};
    ::fast_io::io::perr(u8outbuf, ::uwvm::parameter::details::help_printer{});
    return ::uwvm::cmdline::parameter_return_type::return_imme;
}
