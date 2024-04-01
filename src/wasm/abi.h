#pragma once
#include <cstdint>

namespace uwvm
{
    enum class abi : ::std::uint_least32_t
    {
        detect,
        bare,
        emscripten,
        wasip1,
	wasip2
    };

    inline abi wasm_abi{};

}
