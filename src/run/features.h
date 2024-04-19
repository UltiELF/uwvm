#pragma once

namespace uwvm::features
{
    inline bool enable_exception_handling{};
    inline bool enable_memory64{};
    inline bool enable_multi_memory{};
    inline bool enable_multi_value{};
    inline bool enable_fixed_width_simd{}; // simd 128
    inline bool enable_relaxed_simd{};
    inline bool enable_mutable_globals{};
}
