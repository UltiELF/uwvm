#pragma once
#include <cstdint>
#include "basic_value.h"
#include "value.h"
#include "function.h"

namespace uwvm::wasm
{
    struct initializer_exp
    {
#if 0
        opcode type_opcode;
#endif

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