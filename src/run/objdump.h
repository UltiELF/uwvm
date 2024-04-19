#pragma once
#include <fast_io.h>
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
}  // namespace uwvm
