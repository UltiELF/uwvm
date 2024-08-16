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
                                u8"Interpreter backtrace:"
                                u8"\033[0m");

        auto const lfbegin{::uwvm::global_wasm_module.functionsec.types.cbegin()};

        ::std::size_t c{};
        for(auto const i: ::uwvm::vm::interpreter::int_call_stack.get_container())
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\n"
                                u8"\033[35m" 
                                u8"(",
                                c++,
                                u8") "
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"func[",
                                static_cast<::std::size_t>(i - lfbegin),
                                u8"]" 
                                u8"\033[0m");

            if(i->custom_name_begin) { ::fast_io::io::perr(::uwvm::u8err, u8" : ", ::fast_io::mnp::strvw(i->custom_name_begin, i->custom_name_end)); }
        }
        ::fast_io::io::perr(::uwvm::u8err, u8"\n\n");
    }
}
