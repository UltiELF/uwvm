#pragma once
#include <cstdint>

namespace uwvm
{
    enum class mode : ::std::uint_fast8_t
    {
        objdump,
        interpreter,
#if UWVM_ENABLE_UNCHECKED_INTERPRETER &&                                                                                                                                                   \
    ((defined(_WIN32) || defined(__CYGWIN__)) ||                                                                                                               \
     (!defined(__NEWLIB__) && !defined(__MSDOS__) && (!defined(__wasm__) || (defined(__wasi__) && defined(_WASI_EMULATED_MMAN))) && __has_include(<sys/mman.h>)))
        unchecked_interpreter
#endif
    };

}  // namespace uwvm
