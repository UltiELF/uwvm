#pragma once
#include <cstdint>
#include "basic_value.h"
#include "value.h"
#include "function.h"

namespace uwvm::wasm
{
    struct initializer_exp
    {
        enum class init_type : ::std::uint_fast16_t
        {
            i32_const = 0x0041,
            i64_const = 0x0042,
            f32_const = 0x0043,
            f64_const = 0x0044,
            v128_const = 0xfd02,
            global_get = 0x0023,
            ref_null = 0x00d0,
            ref_func = 0x00d2,
            invalid = 0xffff
        };

        union
        {
            init_type type;
#if 0
            opcode typeOpcode;
#endif
        };

        union
        {
            ::uwvm::wasm::wasm_i32 i32;
            ::uwvm::wasm::wasm_i64 i64;
            ::uwvm::wasm::wasm_f32 f32;
            ::uwvm::wasm::wasm_f64 f64;
            ::uwvm::wasm::wasm_v128 v128;
            ::uwvm::wasm::local_function_type const* ref;
            ::uwvm::wasm::value_type null_reftype;
        };
    };

}  // namespace uwvm::wasm
