#include <io_device.h>

#include "int_bt.h"
#include "astrun.h"

void uwvm::vm::interpreter::int_bt() noexcept
{
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
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"[info] "
                                u8"Interpreter backtrace:\n"
                                u8"\033[0m");

        ::std::size_t c{};
        for(auto const& i: ::uwvm::vm::interpreter::int_call_stack.get_container())
        {
            ::fast_io::io::perrln(::uwvm::u8err,
                                  u8"\033[35m" u8"(",
                                  c++,
                                  u8") "
#ifdef __MSDOS__
                                  u8"\033[37m"
#else
                                  u8"\033[97m"
#endif
                                  ,
                                  i);
        }
        ::fast_io::io::perr(::uwvm::u8err, u8"\n" u8"\033[0m");
    }
}
