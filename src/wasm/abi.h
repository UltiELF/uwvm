#pragma once
#include <cstdint>

namespace uwvm::wasm
{
    enum class abi : ::std::uint_fast32_t
    {
        detect,
        bare,
        emscripten,
        wasip1,
        wasip2
    };

}  // namespace uwvm::wasm
