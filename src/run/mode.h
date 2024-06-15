#pragma once
#include <cstdint>

namespace uwvm
{
    enum class mode : ::std::uint_fast8_t
    {
        objdump, // utf-8
        u16objdump,
        u32objdump
    };

}  // namespace uwvm
