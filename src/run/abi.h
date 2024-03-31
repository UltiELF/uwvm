#pragma once
#include <cstdint>

namespace uwvm
{
    enum class abi : ::std::uint_least32_t
    {
        detect,
        bare,
        emscripten,
        gojs,
        wasi
    };

    inline abi wasm_abi{};
    inline ::std::uint_least32_t wasm_abi_version{};

}
