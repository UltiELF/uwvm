#pragma once
#include <cstdint>
#include <cstddef>

namespace uwvm::wasm
{
    constexpr inline ::std::uint_least64_t maxMemory32Pages{static_cast<::std::uint_least64_t>(0x01) << 16};  // 2^16 pages -> 2^32 bytes
    constexpr inline ::std::uint_least64_t maxMemory64Pages{static_cast<::std::uint_least64_t>(0x01) << 48};  // 2^48 pages -> 2^64 bytes
    constexpr inline ::std::uint_least64_t maxTable32Elems{UINT_LEAST32_MAX};
    constexpr inline ::std::uint_least64_t maxTable64Elems{UINT_LEAST64_MAX};
    constexpr inline ::std::size_t numBytesPerPage{65536};
    constexpr inline ::std::size_t numBytesPerPageLog2{16};
    constexpr inline ::std::size_t numBytesTaggedPerPage{numBytesPerPage / 16u};
    constexpr inline ::std::size_t numBytesTaggedPerPageLog2{12};
    constexpr inline ::std::size_t maxReturnValues{16};

    constexpr inline ::std::uint_least64_t maxMemory64WASMBytes{static_cast<::std::uint_least64_t>(0x01) << 48};  // 256TB
    constexpr inline ::std::uint_least64_t maxMemory64WASMPages{maxMemory64WASMBytes >> numBytesPerPageLog2};
    constexpr inline ::std::uint_least64_t maxMemory64WASMMask{maxMemory64WASMBytes - 1u};

}
