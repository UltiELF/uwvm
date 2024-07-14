#pragma once
#if defined(_MSC_VER)
    #include <stacktrace>
#elif !defined(__MSDOS__) && !defined(__wasm__)
    #include <libunwind.h>
#endif
#include <fast_io.h>
#include <io_device.h>

namespace uwvm
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    inline void
        backtrace() noexcept
    {
#if defined(_MSC_VER)
        auto const bt{::std::stacktrace::current()};
        ::std::size_t counter{};
        for(auto const& i: bt) { ::fast_io::io::perr(::uwvm::u8err, u8"[", counter++, u8"] (", ::fast_io::mnp::code_cvt(i.description()), u8")\n"); }
        ::fast_io::io::perrln(::uwvm::u8err);
#elif !defined(__MSDOS__) && !defined(__wasm__)
        ::unw_cursor_t cursor{};
        ::unw_context_t context{};

        ::unw_getcontext(__builtin_addressof(context));
        ::unw_init_local(__builtin_addressof(cursor), __builtin_addressof(context));

        ::std::size_t counter{};
        while(::unw_step(__builtin_addressof(cursor)) > 0)
        {
            ::unw_word_t offset{};
            ::unw_word_t pc{};

            char8_t fname[1024];
            fname[0] = u8'\0';

            ::unw_get_reg(__builtin_addressof(cursor), UNW_REG_IP, __builtin_addressof(pc));

            ::unw_get_proc_name(__builtin_addressof(cursor), reinterpret_cast<char*>(fname), sizeof(fname), __builtin_addressof(offset));

            ::fast_io::io::perr(::uwvm::u8err,
                                u8"[",
                                counter++,
                                u8"] (",
                                ::fast_io::mnp::os_c_str(fname),
                                u8"+",
                                ::fast_io::mnp::addrvw(offset),
                                u8") [",
                                ::fast_io::mnp::addrvw(pc),
                                u8"]\n");
        }
        ::fast_io::io::perrln(::uwvm::u8err);
#endif
    }
}
