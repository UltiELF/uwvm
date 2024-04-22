#pragma once
#include <cstdint>
#include <unfinished.h> // to do

#include "types/basic_value.h"

namespace uwvm::wasm
{
    // mvp or prefix
    enum class op_basic : ::uwvm::wasm::op_basic_type  // u8
    {
        // MVP
        global_get = 0x23,

        i32_const = 0x41,
        i64_const = 0x42,
        f32_const = 0x43,
        f64_const = 0x44,

        ref_null = 0xd0,
        ref_func = 0xd2,

        // prefix
        simd_prefix = 0xfd,
    };

    // fixed width simd
    enum class op_exten_simd : ::uwvm::wasm::op_exten_type  // u32
    {
        v128_const = 0x02,
    };

    struct opcode
    {
        ::uwvm::wasm::op_exten_type extension{};  // extension
        op_basic op{};                            // mvp or prefix
    };

    inline constexpr bool is_op_mvp(op_basic opp) noexcept
    { 
        // to do
        switch(opp)
        {
            default: return false;
        }
    }

    inline constexpr bool is_op_prefix(op_basic opp) noexcept
    {
        switch(opp)
        {
            // prefix
            case op_basic::simd_prefix: return true;

            // MVP
            default: return false;
        }
    }

    inline constexpr bool is_initializer_type(opcode opc) noexcept
    {
        switch(opc.op)
        {
            // MVP
            case op_basic::global_get: [[fallthrough]];
            case op_basic::i32_const: [[fallthrough]];
            case op_basic::i64_const: [[fallthrough]];
            case op_basic::f32_const: [[fallthrough]];
            case op_basic::f64_const: [[fallthrough]];
            case op_basic::ref_null: [[fallthrough]];
            case op_basic::ref_func: return true;

            // prefix
            case op_basic::simd_prefix:
            {
                switch(static_cast<op_exten_simd>(opc.extension))
                {
                    case op_exten_simd::v128_const: return true;
                    default: return false;
                }
            }
            default: return false;
        }
    }
}  // namespace uwvm::wasm
