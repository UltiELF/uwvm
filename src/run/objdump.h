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
#ifndef __AVR__
        ::fast_io::u16native_io_observer
#else
        ::fast_io::u16c_io_observer
#endif
            const u16out{::uwvm::u8out.native_handle()};

        ::fast_io::basic_obuf<decltype(u16out)> u16outbuf{};
        ::fast_io::io::print(u16outbuf, objdump(::uwvm::global_wasm_module));
    }

    inline void u32objdump() noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer objdump_timer{u8"uwvm: [timer] u32objdump"};
#endif
#ifndef __AVR__
        ::fast_io::u32native_io_observer
#else
        ::fast_io::u32c_io_observer
#endif
            const u32out{::uwvm::u8out.native_handle()};
        ::fast_io::basic_obuf<decltype(u32out)> u32outbuf{};
        ::fast_io::io::print(u32outbuf, objdump(::uwvm::global_wasm_module));
    }
}  // namespace uwvm
