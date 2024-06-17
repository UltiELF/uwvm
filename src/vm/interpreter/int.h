#pragma once
#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include "astgen.h"
#include "aststorge.h"
#include "../../run/wasm_file.h"
#include "../wasm.h"

namespace uwvm::vm::interpreter
{
    inline void interpret() noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer int_timer{u8"uwvm: [timer] interpret"};
#endif
        auto const& wasmmod{::uwvm::global_wasm_module};

        ::uwvm::vm::start_func = ::uwvm::vm::get_start_func(wasmmod);

        if(::uwvm::vm::start_func == static_cast<::std::size_t>(-1)) [[unlikely]]
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
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"Cannot find function: start.\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
    }

}  // namespace uwvm::vm::interpreter
