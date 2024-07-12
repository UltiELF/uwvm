#pragma once

namespace uwvm::features
{
    inline bool enable_exception_handling{};
    inline bool enable_memory64{};
    inline bool enable_multi_memory{};
    inline bool enable_multi_value{};
    inline bool enable_fixed_width_simd{}; // simd 128
    inline bool enable_relaxed_simd{};
    inline constexpr bool enable_mutable_globals{true};
    inline constexpr bool enable_reference_types{true};
    inline bool enable_thread{};
    inline bool enable_custom_page_sizes{};
    inline constexpr bool enable_sign_extension_ops{true};
}
