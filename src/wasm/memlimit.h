#pragma once
#include <cstdint>
#include <cstddef>

namespace uwvm::wasm
{
    inline constexpr ::std::uint_least64_t max_memory32_pages{static_cast<::std::uint_least64_t>(0x01) << 16};  // 2^16 pages -> 2^32 bytes
    inline constexpr ::std::uint_least64_t max_memory64_pages{static_cast<::std::uint_least64_t>(0x01) << 48};   // 2^48 pages -> 2^64 bytes
    inline constexpr ::std::uint_least64_t max_table32_elems{UINT_LEAST32_MAX};
    inline constexpr ::std::uint_least64_t max_table64_elems{UINT_LEAST64_MAX};

    inline constexpr ::std::size_t max_return_values{16};

    inline constexpr ::std::size_t num_bytes_per_page{65536};
    inline constexpr ::std::size_t num_bytes_per_page_log2{16};
    inline constexpr ::std::size_t num_bytes_tagged_per_page{num_bytes_per_page / 16u};
    inline constexpr ::std::size_t num_bytes_tagged_per_page_log2{12};

    // future🦄
    inline constexpr ::std::uint_least64_t num_bytes_per_large_page{static_cast<::std::uint_least64_t>(0x01) << 34};
    inline constexpr ::std::uint_least64_t num_bytes_per_large_page_log2{34};

    inline constexpr ::std::uint_least64_t max_memory64_wasm_bytes{static_cast<::std::uint_least64_t>(0x01) << 40};  // 1TB
    inline constexpr ::std::uint_least64_t max_memory64_wasm_pages{max_memory64_wasm_bytes >> num_bytes_per_page_log2};
    inline constexpr ::std::uint_least64_t max_memory64_wasm_mask{max_memory64_wasm_bytes - 1u};

}  // namespace uwvm::wasm
