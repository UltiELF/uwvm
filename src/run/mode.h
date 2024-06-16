#pragma once
#include <cstdint>

namespace uwvm
{
    enum class mode : ::std::uint_fast8_t
    {
        objdump,
        interpreter,
    };

}  // namespace uwvm
