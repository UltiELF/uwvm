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
        table_get = 0x25,
        table_set = 0x26,

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
        memory_size = 0x3f,
        memory_grow = 0x40,

        // Noncontrolled
        nop = 0x01,

        // Literals
        i32_const = 0x41,
        i64_const = 0x42,
        f32_const = 0x43,
        f64_const = 0x44,

        // Comparisons
        i32_eqz = 0x45,
        i32_eq = 0x46,
        i32_ne = 0x47,
        i32_lt_s = 0x48,
        i32_lt_u = 0x49,
        i32_gt_s = 0x4a,
        i32_gt_u = 0x4b,
        i32_le_s = 0x4c,
        i32_le_u = 0x4d,
        i32_ge_s = 0x4e,
        i32_ge_u = 0x4f,
        i64_eqz = 0x50,
        i64_eq = 0x51,
        i64_ne = 0x52,
        i64_lt_s = 0x53,
        i64_lt_u = 0x54,
        i64_gt_s = 0x55,
        i64_gt_u = 0x56,
        i64_le_s = 0x57,
        i64_le_u = 0x58,
        i64_ge_s = 0x59,
        i64_ge_u = 0x5a,
        f32_eq = 0x5b,
        f32_ne = 0x5c,
        f32_lt = 0x5d,
        f32_gt = 0x5e,
        f32_le = 0x5f,
        f32_ge = 0x60,
        f64_eq = 0x61,
        f64_ne = 0x62,
        f64_lt = 0x63,
        f64_gt = 0x64,
        f64_le = 0x65,
        f64_ge = 0x66,

        // i32 arithmetic
        i32_clz = 0x67,
        i32_ctz = 0x68,
        i32_popcnt = 0x69,
        i32_add = 0x6a,
        i32_sub = 0x6b,
        i32_mul = 0x6c,
        i32_div_s = 0x6d,
        i32_div_u = 0x6e,
        i32_rem_s = 0x6f,
        i32_rem_u = 0x70,
        i32_and = 0x71,
        i32_or = 0x72,
        i32_xor = 0x73,
        i32_shl = 0x74,
        i32_shr_s = 0x75,
        i32_shr_u = 0x76,
        i32_rotl = 0x77,
        i32_rotr = 0x78,

        // i64 arithmetic
        i64_clz = 0x79,
        i64_ctz = 0x7a,
        i64_popcnt = 0x7b,
        i64_add = 0x7c,
        i64_sub = 0x7d,
        i64_mul = 0x7e,
        i64_div_s = 0x7f,
        i64_div_u = 0x80,
        i64_rem_s = 0x81,
        i64_rem_u = 0x82,
        i64_and = 0x83,
        i64_or = 0x84,
        i64_xor = 0x85,
        i64_shl = 0x86,
        i64_shr_s = 0x87,
        i64_shr_u = 0x88,
        i64_rotl = 0x89,
        i64_rotr = 0x8a,

        // f32 arithmetic
        f32_abs = 0x8b,
        f32_neg = 0x8c,
        f32_ceil = 0x8d,
        f32_floor = 0x8e,
        f32_trunc = 0x8f,
        f32_nearest = 0x90,
        f32_sqrt = 0x91,
        f32_add = 0x92,
        f32_sub = 0x93,
        f32_mul = 0x94,
        f32_div = 0x95,
        f32_min = 0x96,
        f32_max = 0x97,
        f32_copysign = 0x98,

        // f64 arithmetic
        f64_abs = 0x99,
        f64_neg = 0x9a,
        f64_ceil = 0x9b,
        f64_floor = 0x9c,
        f64_trunc = 0x9d,
        f64_nearest = 0x9e,
        f64_sqrt = 0x9f,
        f64_add = 0xa0,
        f64_sub = 0xa1,
        f64_mul = 0xa2,
        f64_div = 0xa3,
        f64_min = 0xa4,
        f64_max = 0xa5,
        f64_copysign = 0xa6,

        // Conversions
        i32_wrap_i64 = 0xa7,
        i32_trunc_f32_s = 0xa8,
        i32_trunc_f32_u = 0xa9,
        i32_trunc_f64_s = 0xaa,
        i32_trunc_f64_u = 0xab,
        i64_extend_i32_s = 0xac,
        i64_extend_i32_u = 0xad,
        i64_trunc_f32_s = 0xae,
        i64_trunc_f32_u = 0xaf,
        i64_trunc_f64_s = 0xb0,
        i64_trunc_f64_u = 0xb1,
        f32_convert_i32_s = 0xb2,
        f32_convert_i32_u = 0xb3,
        f32_convert_i64_s = 0xb4,
        f32_convert_i64_u = 0xb5,
        f32_demote_f64 = 0xb6,
        f64_convert_i32_s = 0xb7,
        f64_convert_i32_u = 0xb8,
        f64_convert_i64_s = 0xb9,
        f64_convert_i64_u = 0xba,
        f64_promote_f32 = 0xbb,
        i32_reinterpret_f32 = 0xbc,
        i64_reinterpret_f64 = 0xbd,
        f32_reinterpret_i32 = 0xbe,
        f64_reinterpret_i64 = 0xbf,

        // 8- and 16-bit sign extension operators
        i32_extend8_s = 0xc0,
        i32_extend16_s = 0xc1,
        i64_extend8_s = 0xc2,
        i64_extend16_s = 0xc3,
        i64_extend32_s = 0xc4,

        // Reference type operators
        ref_func = 0xd2,

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

    enum class op_exten_atomic : ::uwvm::wasm::op_exten_type  // 0x
    {
        // Atomic wait/wake
        memory_atomic_notify = 0x00,
        memory_atomic_wait32 = 0x01,
        memory_atomic_wait64 = 0x02,

        // Atomic load/store
        i32_atomic_load = 0x10,
        i64_atomic_load = 0x11,
        i32_atomic_load8_u = 0x12,
        i32_atomic_load16_u = 0x13,
        i64_atomic_load8_u = 0x14,
        i64_atomic_load16_u = 0x15,
        i64_atomic_load32_u = 0x16,
        i32_atomic_store = 0x17,
        i64_atomic_store = 0x18,
        i32_atomic_store8 = 0x19,
        i32_atomic_store16 = 0x1a,
        i64_atomic_store8 = 0x1b,
        i64_atomic_store16 = 0x1c,
        i64_atomic_store32 = 0x1d,

        // Atomic read-modify-write
        i32_atomic_rmw_add = 0x1e,
        i64_atomic_rmw_add = 0x1f,
        i32_atomic_rmw8_add_u = 0x20,
        i32_atomic_rmw16_add_u = 0x21,
        i64_atomic_rmw8_add_u = 0x22,
        i64_atomic_rmw16_add_u = 0x23,
        i64_atomic_rmw32_add_u = 0x24,
        i32_atomic_rmw_sub = 0x25,
        i64_atomic_rmw_sub = 0x26,
        i32_atomic_rmw8_sub_u = 0x27,
        i32_atomic_rmw16_sub_u = 0x28,
        i64_atomic_rmw8_sub_u = 0x29,
        i64_atomic_rmw16_sub_u = 0x2a,
        i64_atomic_rmw32_sub_u = 0x2b,
        i32_atomic_rmw_and = 0x2c,
        i64_atomic_rmw_and = 0x2d,
        i32_atomic_rmw8_and_u = 0x2e,
        i32_atomic_rmw16_and_u = 0x2f,
        i64_atomic_rmw8_and_u = 0x30,
        i64_atomic_rmw16_and_u = 0x31,
        i64_atomic_rmw32_and_u = 0x32,
        i32_atomic_rmw_or = 0x33,
        i64_atomic_rmw_or = 0x34,
        i32_atomic_rmw8_or_u = 0x35,
        i32_atomic_rmw16_or_u = 0x36,
        i64_atomic_rmw8_or_u = 0x37,
        i64_atomic_rmw16_or_u = 0x38,
        i64_atomic_rmw32_or_u = 0x39,
        i32_atomic_rmw_xor = 0x3a,
        i64_atomic_rmw_xor = 0x3b,
        i32_atomic_rmw8_xor_u = 0x3c,
        i32_atomic_rmw16_xor_u = 0x3d,
        i64_atomic_rmw8_xor_u = 0x3e,
        i64_atomic_rmw16_xor_u = 0x3f,
        i64_atomic_rmw32_xor_u = 0x40,
        i32_atomic_rmw_xchg = 0x41,
        i64_atomic_rmw_xchg = 0x42,
        i32_atomic_rmw8_xchg_u = 0x43,
        i32_atomic_rmw16_xchg_u = 0x44,
        i64_atomic_rmw8_xchg_u = 0x45,
        i64_atomic_rmw16_xchg_u = 0x46,
        i64_atomic_rmw32_xchg_u = 0x47,
        i32_atomic_rmw_cmpxchg = 0x48,
        i64_atomic_rmw_cmpxchg = 0x49,
        i32_atomic_rmw8_cmpxchg_u = 0x4a,
        i32_atomic_rmw16_cmpxchg_u = 0x4b,
        i64_atomic_rmw8_cmpxchg_u = 0x4c,
        i64_atomic_rmw16_cmpxchg_u = 0x4d,
        i64_atomic_rmw32_cmpxchg_u = 0x4e,

        // Atomic fence
        atomic_fence = 0x03
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

        // v128 constant
        v128_const = 0x0c,

        // v128 lane manipulation
        i8x16_shuffle = 0x0d,
        i8x16_swizzle = 0x0e,
        i8x16_splat = 0x0f,
        i16x8_splat = 0x10,
        i32x4_splat = 0x11,
        i64x2_splat = 0x12,
        f32x4_splat = 0x13,
        f64x2_splat = 0x14,
        i8x16_extract_lane_s = 0x15,
        i8x16_extract_lane_u = 0x16,
        i8x16_replace_lane = 0x17,
        i16x8_extract_lane_s = 0x18,
        i16x8_extract_lane_u = 0x19,
        i16x8_replace_lane = 0x1a,
        i32x4_extract_lane = 0x1b,
        i32x4_replace_lane = 0x1c,
        i64x2_extract_lane = 0x1d,
        i64x2_replace_lane = 0x1e,
        f32x4_extract_lane = 0x1f,
        f32x4_replace_lane = 0x20,
        f64x2_extract_lane = 0x21,
        f64x2_replace_lane = 0x22,

        // v128 comparisons
        i8x16_eq = 0x23,
        i8x16_ne = 0x24,
        i8x16_lt_s = 0x25,
        i8x16_lt_u = 0x26,
        i8x16_gt_s = 0x27,
        i8x16_gt_u = 0x28,
        i8x16_le_s = 0x29,
        i8x16_le_u = 0x2a,
        i8x16_ge_s = 0x2b,
        i8x16_ge_u = 0x2c,

        i16x8_eq = 0x2d,
        i16x8_ne = 0x2e,
        i16x8_lt_s = 0x2f,
        i16x8_lt_u = 0x30,
        i16x8_gt_s = 0x31,
        i16x8_gt_u = 0x32,
        i16x8_le_s = 0x33,
        i16x8_le_u = 0x34,
        i16x8_ge_s = 0x35,
        i16x8_ge_u = 0x36,

        i32x4_eq = 0x37,
        i32x4_ne = 0x38,
        i32x4_lt_s = 0x39,
        i32x4_lt_u = 0x3a,
        i32x4_gt_s = 0x3b,
        i32x4_gt_u = 0x3c,
        i32x4_le_s = 0x3d,
        i32x4_le_u = 0x3e,
        i32x4_ge_s = 0x3f,
        i32x4_ge_u = 0x40,

        f32x4_eq = 0x41,
        f32x4_ne = 0x42,
        f32x4_lt = 0x43,
        f32x4_gt = 0x44,
        f32x4_le = 0x45,
        f32x4_ge = 0x46,

        f64x2_eq = 0x47,
        f64x2_ne = 0x48,
        f64x2_lt = 0x49,
        f64x2_gt = 0x4a,
        f64x2_le = 0x4b,
        f64x2_ge = 0x4c,

        // v128 bitwise
        v128_not = 0x4d,
        v128_and = 0x4e,
        v128_andnot = 0x4f,
        v128_or = 0x50,
        v128_xor = 0x51,
        v128_bitselect = 0x52,
        v128_any_true = 0x53,

        f32x4_demote_f64x2_zero = 0x5e,
        f64x2_promote_low_f32x4 = 0x5f,
        // v128 integer arithmetic
        i8x16_abs = 0x60,
        i8x16_neg = 0x61,
        i8x16_popcnt = 0x62,
        i8x16_all_true = 0x63,
        i8x16_bitmask = 0x64,
        i8x16_narrow_i16x8_s = 0x65,
        i8x16_narrow_i16x8_u = 0x66,
        f32x4_ceil = 0x67,
        f32x4_floor = 0x68,
        f32x4_trunc = 0x69,
        f32x4_nearest = 0x6a,
        i8x16_shl = 0x6b,
        i8x16_shr_s = 0x6c,
        i8x16_shr_u = 0x6d,
        i8x16_add = 0x6e,
        i8x16_add_sat_s = 0x6f,
        i8x16_add_sat_u = 0x70,
        i8x16_sub = 0x71,
        i8x16_sub_sat_s = 0x72,
        i8x16_sub_sat_u = 0x73,
        f64x2_ceil = 0x74,
        f64x2_floor = 0x75,
        i8x16_min_s = 0x76,
        i8x16_min_u = 0x77,
        i8x16_max_s = 0x78,
        i8x16_max_u = 0x79,
        f64x2_trunc = 0x7a,
        i8x16_avgr_u = 0x7b,

        i16x8_extadd_pairwise_i8x16_s = 0x7c,
        i16x8_extadd_pairwise_i8x16_u = 0x7d,
        i32x4_extadd_pairwise_i16x8_s = 0x7e,
        i32x4_extadd_pairwise_i16x8_u = 0x7f,

        i16x8_abs = 0x80,
        i16x8_neg = 0x81,
        i16x8_q15mulr_sat_s = 0x82,
        i16x8_all_true = 0x83,
        i16x8_bitmask = 0x84,
        i16x8_narrow_i32x4_s = 0x85,
        i16x8_narrow_i32x4_u = 0x86,
        i16x8_extend_low_i8x16_s = 0x87,
        i16x8_extend_high_i8x16_s = 0x88,
        i16x8_extend_low_i8x16_u = 0x89,
        i16x8_extend_high_i8x16_u = 0x8a,
        i16x8_shl = 0x8b,
        i16x8_shr_s = 0x8c,
        i16x8_shr_u = 0x8d,
        i16x8_add = 0x8e,
        i16x8_add_sat_s = 0x8f,
        i16x8_add_sat_u = 0x90,
        i16x8_sub = 0x91,
        i16x8_sub_sat_s = 0x92,
        i16x8_sub_sat_u = 0x93,

        f64x2_nearest = 0x94,

        i16x8_mul = 0x95,
        i16x8_min_s = 0x96,
        i16x8_min_u = 0x97,
        i16x8_max_s = 0x98,
        i16x8_max_u = 0x99,
        i16x8_avgr_s = 0x9a,
        i16x8_avgr_u = 0x9b,
        i16x8_extmul_low_i8x16_s = 0x9c,
        i16x8_extmul_high_i8x16_s = 0x9d,
        i16x8_extmul_low_i8x16_u = 0x9e,
        i16x8_extmul_high_i8x16_u = 0x9f,

        i32x4_abs = 0xa0,
        i32x4_neg = 0xa1,

        i32x4_all_true = 0xa3,
        i32x4_bitmask = 0xa4,
        i32x4_narrow_i64x2_s = 0xa5,
        i32x4_narrow_i64x2_u = 0xa6,
        i32x4_extend_low_i16x8_s = 0xa7,
        i32x4_extend_high_i16x8_s = 0xa8,
        i32x4_extend_low_i16x8_u = 0xa9,
        i32x4_extend_high_i16x8_u = 0xaa,
        i32x4_shl = 0xab,
        i32x4_shr_s = 0xac,
        i32x4_shr_u = 0xad,
        i32x4_add = 0xae,
        i32x4_add_sat_s = 0xaf,
        i32x4_add_sat_u = 0xb0,
        i32x4_sub = 0xb1,
        i32x4_sub_sat_s = 0xb2,
        i32x4_sub_sat_u = 0xb3,

        i32x4_mul = 0xb5,
        i32x4_min_s = 0xb6,
        i32x4_min_u = 0xb7,
        i32x4_max_s = 0xb8,
        i32x4_max_u = 0xb9,
        i32x4_dot_i16x8_s = 0xba,

        i32x4_extmul_low_i16x8_s = 0xbc,
        i32x4_extmul_high_i16x8_s = 0xbd,
        i32x4_extmul_low_i16x8_u = 0xbe,
        i32x4_extmul_high_i16x8_u = 0xbf,

        i64x2_abs = 0xc0,
        i64x2_neg = 0xc1,
        i64x2_all_true = 0xc3,
        i64x2_bitmask = 0xc4,
        i64x2_narrow_i128x1_s = 0xc5,
        i64x2_narrow_i128x1_u = 0xc6,
        i64x2_extend_low_i32x4_s = 0xc7,
        i64x2_extend_high_i32x4_s = 0xc8,
        i64x2_extend_low_i32x4_u = 0xc9,
        i64x2_extend_high_i32x4_u = 0xca,
        i64x2_shl = 0xcb,
        i64x2_shr_s = 0xcc,
        i64x2_shr_u = 0xcd,
        i64x2_add = 0xce,
        i64x2_add_sat_s = 0xcf,
        i64x2_add_sat_u = 0xd0,
        i64x2_sub = 0xd1,
        i64x2_sub_sat_s = 0xd2,
        i64x2_sub_sat_u = 0xd3,
        i64x2_mul = 0xd5,
        i64x2_eq = 0xd6,
        i64x2_ne = 0xd7,
        i64x2_lt_s = 0xd8,
        i64x2_gt_s = 0xd9,
        i64x2_le_s = 0xda,
        i64x2_ge_s = 0xdb,
        i64x2_extmul_low_i32x4_s = 0xdc,
        i64x2_extmul_high_i32x4_s = 0xdd,
        i64x2_extmul_low_i32x4_u = 0xde,
        i64x2_extmul_high_i32x4_u = 0xdf,

        // v128 floating-point arithmetic
        f32x4_abs = 0xe0,
        f32x4_neg = 0xe1,
        f32x4_round = 0xe2,
        f32x4_sqrt = 0xe3,
        f32x4_add = 0xe4,
        f32x4_sub = 0xe5,
        f32x4_mul = 0xe6,
        f32x4_div = 0xe7,
        f32x4_min = 0xe8,
        f32x4_max = 0xe9,
        f32x4_pmin = 0xea,
        f32x4_pmax = 0xeb,

        f64x2_abs = 0xec,
        f64x2_neg = 0xed,
        f64x2_round = 0xee,
        f64x2_sqrt = 0xef,
        f64x2_add = 0xf0,
        f64x2_sub = 0xf1,
        f64x2_mul = 0xf2,
        f64x2_div = 0xf3,
        f64x2_min = 0xf4,
        f64x2_max = 0xf5,
        f64x2_pmin = 0xf6,
        f64x2_pmax = 0xf7,

        i32x4_trunc_sat_f32x4_s = 0xf8,
        i32x4_trunc_sat_f32x4_u = 0xf9,
        f32x4_convert_i32x4_s = 0xfa,
        f32x4_convert_i32x4_u = 0xfb,
        i32x4_trunc_sat_f64x2_s_zero = 0xfc,
        i32x4_trunc_sat_f64x2_u_zero = 0xfd,
        f64x2_convert_low_i32x4_s = 0xfe,
        f64x2_convert_low_i32x4_u = 0xff
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
        memory_hintstoreztag = 0x2e,

        // Saturating float->int truncation operators
        i32_trunc_sat_f32_s = 0x00,
        i32_trunc_sat_f32_u = 0x01,
        i32_trunc_sat_f64_s = 0x02,
        i32_trunc_sat_f64_u = 0x03,
        i64_trunc_sat_f32_s = 0x04,
        i64_trunc_sat_f32_u = 0x05,
        i64_trunc_sat_f64_s = 0x06,
        i64_trunc_sat_f64_u = 0x07,

        // Bulk memory/table operators
        data_drop = 0x09,
        elem_drop = 0x0d

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
 *	0xfc00, i32_trunc_sat_f32_s
 */
