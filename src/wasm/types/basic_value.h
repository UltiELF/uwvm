#pragma once
#include <cstdint>
#include <cstddef>

namespace uwvm::wasm
{
    // integral
    using wasm_i8 = ::std::int_least8_t;
    using wasm_u8 = ::std::uint_least8_t;

    using wasm_i16 = ::std::int_least16_t;
    using wasm_u16 = ::std::uint_least16_t;

    using wasm_i32 = ::std::int_least32_t;
    using wasm_u32 = ::std::uint_least32_t;

    using wasm_i64 = ::std::int_least64_t;
    using wasm_u64 = ::std::uint_least64_t;

    // floating point
#ifdef __STDCPP_FLOAT32_T__
    using wasm_f32 = _Float32;
#else
    using wasm_f32 = float;
#endif
#ifdef __STDCPP_FLOAT64_T__
    using wasm_f64 = _Float64;
#else
    using wasm_f64 = double;
#endif

    // vector
#if __has_cpp_attribute(__gnu__::__vector_size__)
    using wasm_v128 [[__gnu__::__vector_size__(16)]] = char;
#else
    #if defined(_MSC_VER) && !defined(__clang__)
    __declspec(align(16))
    #endif
    union
    #if __has_cpp_attribute(__gnu__::__aligned__)
        [[__gnu__::__aligned__(16)]]
    #endif
        v128_impl
    {
        wasm_u8 u8x16[16];
        wasm_i8 i8x16[16];

        wasm_u16 u16x8[8];
        wasm_i16 i16x8[8];

        wasm_u32 u32x4[4];
        wasm_i32 i32x4[4];
        wasm_f32 f32x4[4];

        wasm_u64 u64x2[2];
        wasm_i64 i64x2[2];
        wasm_f64 f64x2[2];
    };

    using wasm_v128 = v128_impl;
#endif

    // (b)f16 and f128: future🦄
#if defined(__SIZEOF_FLOAT16__) || defined(__FLOAT16__)
    using wasm_f16 = __float16;
#endif
#if defined(__STDCPP_BFLOAT16_T__)
    using wasm_bf16 = decltype(0.0bf16);
#endif
#if defined(__SIZEOF_FLOAT128__) || defined(__FLOAT128__)
    using wasm_f128 = __float128;
#endif
}  // namespace uwvm::wasm

#if defined(__SIZEOF_FLOAT16__) || defined(__FLOAT16__)
    #define UWVM_SUPPORT_F16
#endif
#if defined(__STDCPP_BFLOAT16_T__)
    #define UWVM_SUPPORT_BF16
#endif
#if defined(__SIZEOF_FLOAT128__) || defined(__FLOAT128__)
    #define UWVM_SUPPORT_F128
#endif
