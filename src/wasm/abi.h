#pragma once
#include <cstdint>
#include <fast_io_dsal/string_view.h>

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

    inline constexpr ::fast_io::u8string_view get_abi_u8name(abi abitype) noexcept
    {
        switch(abitype)
        {
            case uwvm::wasm::abi::detect: return ::fast_io::u8string_view{u8"detect"};
            case uwvm::wasm::abi::bare: return ::fast_io::u8string_view{u8"bare"};
            case uwvm::wasm::abi::emscripten: return ::fast_io::u8string_view{u8"emscripten"};
            case uwvm::wasm::abi::wasip1: return ::fast_io::u8string_view{u8"wasip1"};
            case uwvm::wasm::abi::wasip2: return ::fast_io::u8string_view{u8"wasip2"};
            default: return {};
        }
    }
}  // namespace uwvm::wasm
