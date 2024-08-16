#include <io_device.h>

#include "int_bt.h"
#include "astrun.h"

void uwvm::vm::interpreter::int_bt() noexcept
{
    {
        ::fast_io::io::perr(::uwvm::u8err, u8"\ninterpreter backtrace:\n");

        ::std::size_t c{};
        for(auto const& i: ::uwvm::vm::interpreter::int_call_stack.get_container()) { ::fast_io::io::perrln(::uwvm::u8err, u8"(", c++, u8") ", i); }
        ::fast_io::io::perrln(::uwvm::u8err);
    }
}
