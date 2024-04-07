#pragma once
#include <cstddef>
#include <fast_io.h>

namespace uwvm
{
    inline constexpr bool is_wasm_file_unchecked(::std::byte const* fr) noexcept
    {
        return ::fast_io::freestanding::my_memcmp(fr, u8"\0asm", 4u) == 0;
    }

    inline constexpr ::std::uint_least32_t detect_wasm_version_unchecked(::std::byte const* fr) noexcept
    {
        ::std::uint_least32_t temp{};
        ::fast_io::freestanding::my_memcpy(__builtin_addressof(temp), fr, sizeof(::std::uint_least32_t));
        return ::fast_io::little_endian(temp);
    }
}  // namespace uwvm
