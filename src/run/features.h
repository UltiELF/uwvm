#pragma once

namespace uwvm::features
{
    inline bool enable_exception_handling{};
    inline bool enable_memory64{};
    inline bool enable_multi_memory{};
    inline bool enable_fixed_width_simd{};  // simd 128
    inline bool enable_relaxed_simd{};
    inline bool enable_thread{};
    inline bool enable_custom_page_sizes{};
#ifndef UWVM_DEFAULT_MVP
    inline constexpr bool enable_multi_value{true};
    inline constexpr bool enable_sign_extension_ops{true};
    inline constexpr bool enable_mutable_globals{true};
    inline constexpr bool enable_reference_types{true};
    inline constexpr bool enable_bulk_memory_operations{true};
    inline constexpr bool enable_non_trapping_float_to_int_conversion{true};
#else
    inline bool enable_multi_value{};
    inline bool enable_sign_extension_ops{};
    inline bool enable_mutable_globals{};
    inline bool enable_reference_types{};
    inline bool enable_bulk_memory_operations{};
    inline bool enable_non_trapping_float_to_int_conversion{};
#endif
}  // namespace uwvm::features
