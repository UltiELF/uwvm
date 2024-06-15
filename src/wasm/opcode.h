#pragma once
#include <cstdint>

#include "types/basic_value.h"

namespace uwvm::wasm
{
    // mvp or prefix
    enum class op_basic : ::uwvm::wasm::op_basic_type  // u8
    {
        /***********
         *   MVP   *
         **********/

        // Control
        block = 0x02,
        loop = 0x03,
        if_ = 0x04,
        else_ = 0x05,
        end = 0x0b,
        try_ = 0x06,
        catch_ = 0x07,
        delegate_ = 0x18,
        catch_all = 0x19,

        // Control flow
        unreachable = 0x00,
        br = 0x0c,
        br_if = 0x0d,
        br_table = 0x0e,
        return_ = 0x0f,
        call = 0x10,
        call_indirect = 0x11,

        // Stack manipulation
        drop = 0x1a,

        // Variables
        local_get = 0x20,
        local_set = 0x21,
        local_tee = 0x22,
        global_get = 0x23,
        global_set = 0x24,

        // Table access
        table_get = 0x0025,
        table_set = 0x0026,

        // Exceptions
        throw_ = 0x08,
        rethrow = 0x09,

        // References
        ref_null = 0xd0,
        ref_is_null = 0xd1,

        // Overloaded
        select = 0x1c,

        // Scalar load/store instructions
        i32_load = 0x28,
        i64_load = 0x29,
        f32_load = 0x2a,
        f64_load = 0x2b,
        i32_load8_s = 0x2c,
        i32_load8_u = 0x2d,
        i32_load16_s = 0x2e,
        i32_load16_u = 0x2f,
        i64_load8_s = 0x30,
        i64_load8_u = 0x31,
        i64_load16_s = 0x32,
        i64_load16_u = 0x33,
        i64_load32_s = 0x34,
        i64_load32_u = 0x35,
        i32_store = 0x36,
        i64_store = 0x37,
        f32_store = 0x38,
        f64_store = 0x39,
        i32_store8 = 0x3a,
        i32_store16 = 0x3b,
        i64_store8 = 0x3c,
        i64_store16 = 0x3d,
        i64_store32 = 0x3e,

        // Memory size
        memory_size = 0x003f,
        memory_grow = 0x0040,

        // Noncontrolled
        nop = 0x0001,

        // Literals
        i32_const = 0x41,
        i64_const = 0x42,
        f32_const = 0x43,
        f64_const = 0x44,

        // Comparisons
        i32_eqz = 0x0045,
        i32_eq = 0x0046,
        i32_ne = 0x0047,
        i32_lt_s = 0x0048,
        i32_lt_u = 0x0049,
        i32_gt_s = 0x004a,
        i32_gt_u = 0x004b,
        i32_le_s = 0x004c,
        i32_le_u = 0x004d,
        i32_ge_s = 0x004e,
        i32_ge_u = 0x004f,
        i64_eqz = 0x0050,
        i64_eq = 0x0051,
        i64_ne = 0x0052,
        i64_lt_s = 0x0053,
        i64_lt_u = 0x0054,
        i64_gt_s = 0x0055,
        i64_gt_u = 0x0056,
        i64_le_s = 0x0057,
        i64_le_u = 0x0058,
        i64_ge_s = 0x0059,
        i64_ge_u = 0x005a,
        f32_eq = 0x005b,
        f32_ne = 0x005c,
        f32_lt = 0x005d,
        f32_gt = 0x005e,
        f32_le = 0x005f,
        f32_ge = 0x0060,
        f64_eq = 0x0061,
        f64_ne = 0x0062,
        f64_lt = 0x0063,
        f64_gt = 0x0064,
        f64_le = 0x0065,
        f64_ge = 0x0066,

        // i32 arithmetic
        i32_clz = 0x0067,
        i32_ctz = 0x0068,
        i32_popcnt = 0x0069,
        i32_add = 0x006a,
        i32_sub = 0x006b,
        i32_mul = 0x006c,
        i32_div_s = 0x006d,
        i32_div_u = 0x006e,
        i32_rem_s = 0x006f,
        i32_rem_u = 0x0070,
        i32_and = 0x0071,
        i32_or = 0x0072,
        i32_xor = 0x0073,
        i32_shl = 0x0074,
        i32_shr_s = 0x0075,
        i32_shr_u = 0x0076,
        i32_rotl = 0x0077,
        i32_rotr = 0x0078,

        // i64 arithmetic
        i64_clz = 0x0079,
        i64_ctz = 0x007a,
        i64_popcnt = 0x007b,
        i64_add = 0x007c,
        i64_sub = 0x007d,
        i64_mul = 0x007e,
        i64_div_s = 0x007f,
        i64_div_u = 0x0080,
        i64_rem_s = 0x0081,
        i64_rem_u = 0x0082,
        i64_and = 0x0083,
        i64_or = 0x0084,
        i64_xor = 0x0085,
        i64_shl = 0x0086,
        i64_shr_s = 0x0087,
        i64_shr_u = 0x0088,
        i64_rotl = 0x0089,
        i64_rotr = 0x008a,

        // f32 arithmetic
        f32_abs = 0x008b,
        f32_neg = 0x008c,
        f32_ceil = 0x008d,
        f32_floor = 0x008e,
        f32_trunc = 0x008f,
        f32_nearest = 0x0090,
        f32_sqrt = 0x0091,
        f32_add = 0x0092,
        f32_sub = 0x0093,
        f32_mul = 0x0094,
        f32_div = 0x0095,
        f32_min = 0x0096,
        f32_max = 0x0097,
        f32_copysign = 0x0098,

        // f64 arithmetic
        f64_abs = 0x0099,
        f64_neg = 0x009a,
        f64_ceil = 0x009b,
        f64_floor = 0x009c,
        f64_trunc = 0x009d,
        f64_nearest = 0x009e,
        f64_sqrt = 0x009f,
        f64_add = 0x00a0,
        f64_sub = 0x00a1,
        f64_mul = 0x00a2,
        f64_div = 0x00a3,
        f64_min = 0x00a4,
        f64_max = 0x00a5,
        f64_copysign = 0x00a6,

        // Conversions
        i32_wrap_i64 = 0x00a7,
        i32_trunc_f32_s = 0x00a8,
        i32_trunc_f32_u = 0x00a9,
        i32_trunc_f64_s = 0x00aa,
        i32_trunc_f64_u = 0x00ab,
        i64_extend_i32_s = 0x00ac,
        i64_extend_i32_u = 0x00ad,
        i64_trunc_f32_s = 0x00ae,
        i64_trunc_f32_u = 0x00af,
        i64_trunc_f64_s = 0x00b0,
        i64_trunc_f64_u = 0x00b1,
        f32_convert_i32_s = 0x00b2,
        f32_convert_i32_u = 0x00b3,
        f32_convert_i64_s = 0x00b4,
        f32_convert_i64_u = 0x00b5,
        f32_demote_f64 = 0x00b6,
        f64_convert_i32_s = 0x00b7,
        f64_convert_i32_u = 0x00b8,
        f64_convert_i64_s = 0x00b9,
        f64_convert_i64_u = 0x00ba,
        f64_promote_f32 = 0x00bb,
        i32_reinterpret_f32 = 0x00bc,
        i64_reinterpret_f64 = 0x00bd,
        f32_reinterpret_i32 = 0x00be,
        f64_reinterpret_i64 = 0x00bf,

        // 8- and 16-bit sign extension operators
        i32_extend8_s = 0x00c0,
        i32_extend16_s = 0x00c1,
        i64_extend8_s = 0x00c2,
        i64_extend16_s = 0x00c3,
        i64_extend32_s = 0x00c4,

        // Reference type operators
        ref_func = 0x00d2,

        /**********
         * prefix *
         **********/
        simd_interleaved = 0xff,
        simd = 0xfd,
        reference_types = 0xfc,
        bulkMemory = 0xfc,
    };

    enum class op_exten_simd_interleaved : ::uwvm::wasm::op_exten_type  // 0xff
    {
        // V128 interleaved load/store instructions
        v8x16_load_interleaved_2 = 0x00,
        v8x16_load_interleaved_3 = 0x01,
        v8x16_load_interleaved_4 = 0x02,
        v16x8_load_interleaved_2 = 0x03,
        v16x8_load_interleaved_3 = 0x04,
        v16x8_load_interleaved_4 = 0x05,
        v32x4_load_interleaved_2 = 0x06,
        v32x4_load_interleaved_3 = 0x07,
        v32x4_load_interleaved_4 = 0x08,
        v64x2_load_interleaved_2 = 0x09,
        v64x2_load_interleaved_3 = 0x0a,
        v64x2_load_interleaved_4 = 0x0b,
        v8x16_store_interleaved_2 = 0x0c,
        v8x16_store_interleaved_3 = 0x0d,
        v8x16_store_interleaved_4 = 0x0e,
        v16x8_store_interleaved_2 = 0x0f,
        v16x8_store_interleaved_3 = 0x10,
        v16x8_store_interleaved_4 = 0x11,
        v32x4_store_interleaved_2 = 0x12,
        v32x4_store_interleaved_3 = 0x13,
        v32x4_store_interleaved_4 = 0x14,
        v64x2_store_interleaved_2 = 0x15,
        v64x2_store_interleaved_3 = 0x16,
        v64x2_store_interleaved_4 = 0x17

    };

    enum class op_exten_atomic : ::uwvm::wasm::op_exten_type  // 0xfe
    {
        // Atomic wait/wake
        memory_atomic_notify = 0xfe00,
        memory_atomic_wait32 = 0xfe01,
        memory_atomic_wait64 = 0xfe02,

        // Atomic load/store
        i32_atomic_load = 0xfe10,
        i64_atomic_load = 0xfe11,
        i32_atomic_load8_u = 0xfe12,
        i32_atomic_load16_u = 0xfe13,
        i64_atomic_load8_u = 0xfe14,
        i64_atomic_load16_u = 0xfe15,
        i64_atomic_load32_u = 0xfe16,
        i32_atomic_store = 0xfe17,
        i64_atomic_store = 0xfe18,
        i32_atomic_store8 = 0xfe19,
        i32_atomic_store16 = 0xfe1a,
        i64_atomic_store8 = 0xfe1b,
        i64_atomic_store16 = 0xfe1c,
        i64_atomic_store32 = 0xfe1d,

        // Atomic read-modify-write
        i32_atomic_rmw_add = 0xfe1e,
        i64_atomic_rmw_add = 0xfe1f,
        i32_atomic_rmw8_add_u = 0xfe20,
        i32_atomic_rmw16_add_u = 0xfe21,
        i64_atomic_rmw8_add_u = 0xfe22,
        i64_atomic_rmw16_add_u = 0xfe23,
        i64_atomic_rmw32_add_u = 0xfe24,
        i32_atomic_rmw_sub = 0xfe25,
        i64_atomic_rmw_sub = 0xfe26,
        i32_atomic_rmw8_sub_u = 0xfe27,
        i32_atomic_rmw16_sub_u = 0xfe28,
        i64_atomic_rmw8_sub_u = 0xfe29,
        i64_atomic_rmw16_sub_u = 0xfe2a,
        i64_atomic_rmw32_sub_u = 0xfe2b,
        i32_atomic_rmw_and = 0xfe2c,
        i64_atomic_rmw_and = 0xfe2d,
        i32_atomic_rmw8_and_u = 0xfe2e,
        i32_atomic_rmw16_and_u = 0xfe2f,
        i64_atomic_rmw8_and_u = 0xfe30,
        i64_atomic_rmw16_and_u = 0xfe31,
        i64_atomic_rmw32_and_u = 0xfe32,
        i32_atomic_rmw_or = 0xfe33,
        i64_atomic_rmw_or = 0xfe34,
        i32_atomic_rmw8_or_u = 0xfe35,
        i32_atomic_rmw16_or_u = 0xfe36,
        i64_atomic_rmw8_or_u = 0xfe37,
        i64_atomic_rmw16_or_u = 0xfe38,
        i64_atomic_rmw32_or_u = 0xfe39,
        i32_atomic_rmw_xor = 0xfe3a,
        i64_atomic_rmw_xor = 0xfe3b,
        i32_atomic_rmw8_xor_u = 0xfe3c,
        i32_atomic_rmw16_xor_u = 0xfe3d,
        i64_atomic_rmw8_xor_u = 0xfe3e,
        i64_atomic_rmw16_xor_u = 0xfe3f,
        i64_atomic_rmw32_xor_u = 0xfe40,
        i32_atomic_rmw_xchg = 0xfe41,
        i64_atomic_rmw_xchg = 0xfe42,
        i32_atomic_rmw8_xchg_u = 0xfe43,
        i32_atomic_rmw16_xchg_u = 0xfe44,
        i64_atomic_rmw8_xchg_u = 0xfe45,
        i64_atomic_rmw16_xchg_u = 0xfe46,
        i64_atomic_rmw32_xchg_u = 0xfe47,
        i32_atomic_rmw_cmpxchg = 0xfe48,
        i64_atomic_rmw_cmpxchg = 0xfe49,
        i32_atomic_rmw8_cmpxchg_u = 0xfe4a,
        i32_atomic_rmw16_cmpxchg_u = 0xfe4b,
        i64_atomic_rmw8_cmpxchg_u = 0xfe4c,
        i64_atomic_rmw16_cmpxchg_u = 0xfe4d,
        i64_atomic_rmw32_cmpxchg_u = 0xfe4e

    };

    // fixed width simd
    enum class op_exten_simd : ::uwvm::wasm::op_exten_type  // 0xfd
    {
        // v128 load/store
        v128_load = 0x00,
        v128_load8x8_s = 0x01,
        v128_load8x8_u = 0x02,
        v128_load16x4_s = 0x03,
        v128_load16x4_u = 0x04,
        v128_load32x2_s = 0x05,
        v128_load32x2_u = 0x06,
        v128_load8_splat = 0x07,
        v128_load16_splat = 0x08,
        v128_load32_splat = 0x09,
        v128_load64_splat = 0x0a,
        v128_store = 0x0b,
        v128_load8_lane = 0x54,
        v128_load16_lane = 0x55,
        v128_load32_lane = 0x56,
        v128_load64_lane = 0x57,
        v128_store8_lane = 0x58,
        v128_store16_lane = 0x59,
        v128_store32_lane = 0x5a,
        v128_store64_lane = 0x5b,
        v128_load32_zero = 0x5c,
        v128_load64_zero = 0x5d,

        v128_const = 0x0c,
    };

    enum class op_exten_reftype : ::uwvm::wasm::op_exten_type  // 0xfc
    {
        // Table access
        table_grow = 0x0f,
        table_fill = 0x11,

        // Bulk memory/table operators
        memory_init = 0x08,
        memory_copy = 0x0a,
        memory_fill = 0x0b,
        table_init = 0x0c,
        table_copy = 0x0e,
        table_size = 0x10,

        // Memory Tag
        memory_randomtag = 0x20,
        memory_copytag = 0x21,
        memory_subtag = 0x22,
        memory_loadtag = 0x23,
        memory_storetag = 0x24,
        memory_storeztag = 0x25,
        memory_randomstoretag = 0x28,
        memory_randomstoreztag = 0x29,
        memory_hinttag = 0x2c,
        memory_hintstoretag = 0x2d,
        memory_hintstoreztag = 0x2e

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
            case op_basic::simd: return true;

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
            case op_basic::simd:
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

/*
* to do
*	visitOp(0xfc00, i32_trunc_sat_f32_s     
*/
