#pragma once
#include <cstdint>
#include <cstddef>

namespace uwvm::wasm
{
    inline constexpr ::std::uint_least64_t maxMemory32Pages{static_cast<::std::uint_least64_t>(0x01) << 16};  // 2^16 pages -> 2^32 bytes
    inline constexpr ::std::uint_least64_t maxMemory64Pages{static_cast<::std::uint_least64_t>(0x01) << 48};  // 2^48 pages -> 2^64 bytes
    inline constexpr ::std::uint_least64_t maxTable32Elems{UINT_LEAST32_MAX};
    inline constexpr ::std::uint_least64_t maxTable64Elems{UINT_LEAST64_MAX};
    inline constexpr ::std::size_t numBytesPerPage{65536};
    inline constexpr ::std::size_t numBytesPerPageLog2{16};
    inline constexpr ::std::size_t numBytesTaggedPerPage{numBytesPerPage / 16u};
    inline constexpr ::std::size_t numBytesTaggedPerPageLog2{12};
    inline constexpr ::std::size_t maxReturnValues{16};

    inline constexpr ::std::uint_least64_t maxMemory64WASMBytes{static_cast<::std::uint_least64_t>(0x01) << 48};  // 256TB
    inline constexpr ::std::uint_least64_t maxMemory64WASMPages{maxMemory64WASMBytes >> numBytesPerPageLog2};
    inline constexpr ::std::uint_least64_t maxMemory64WASMMask{maxMemory64WASMBytes - 1u};

}  // namespace uwvm::wasm
