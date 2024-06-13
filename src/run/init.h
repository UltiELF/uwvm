#pragma once
#include <cstddef>
#include <fast_io.h>
#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include "../wasm/custom.h"

namespace uwvm
{
    inline void init() noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer init_timer{u8"uwvm: [timer] init"};
#endif

#if 0
        ::uwvm::wasm::custom::init_builtin_custom_section();
#endif
    }

}  // namespace uwvm
