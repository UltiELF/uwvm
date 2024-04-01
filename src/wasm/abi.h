#pragma once
#include <cstdint>

namespace uwvm::wasm
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
