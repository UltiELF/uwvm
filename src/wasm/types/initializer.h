#pragma once
#include <cstdint>
#include "basic_value.h"
#include "value.h"
#include "../opcode.h"

namespace uwvm::wasm
{
    struct initializer_exp
    {
        union
        {
            ::uwvm::wasm::wasm_i32 i32;
            ::uwvm::wasm::wasm_i64 i64;
            ::uwvm::wasm::wasm_f32 f32;
            ::uwvm::wasm::wasm_f64 f64;
            ::uwvm::wasm::wasm_v128 v128;
            ::std::size_t ref; // func, global
            ::uwvm::wasm::value_type null_reftype;
        };

        ::uwvm::wasm::opcode type_opcode{};
    };

}  // namespace uwvm::wasm
