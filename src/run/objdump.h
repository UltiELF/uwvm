#pragma once
#include <fast_io.h>
#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include "../wasm/objdump.h"
#include "wasm_file.h"

namespace uwvm
{
    inline void u8objdump() noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer objdump_timer{u8"uwvm: [timer] objdump"};
#endif
        ::fast_io::basic_obuf<decltype(::uwvm::u8out)> u8outbuf{::uwvm::u8out};
        ::fast_io::io::print(u8outbuf, objdump(::uwvm::global_wasm_module));
    }

    inline void u16objdump() noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer objdump_timer{u8"uwvm: [timer] u16objdump"};
#endif
        ::fast_io::basic_obuf<decltype(::fast_io::u16c_stdout())> u16outbuf{::fast_io::u16c_stdout()};
        ::fast_io::io::print(u16outbuf, objdump(::uwvm::global_wasm_module));
    }

    inline void u32objdump() noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer objdump_timer{u8"uwvm: [timer] u32objdump"};
#endif
        ::fast_io::basic_obuf<decltype(::fast_io::u32c_stdout())> u32outbuf{::fast_io::u32c_stdout()};
        ::fast_io::io::print(u32outbuf, objdump(::uwvm::global_wasm_module));
    }
}  // namespace uwvm
