#pragma once
#include <cstddef>
#include "../../wasm/types.h"

namespace uwvm::vm::unchecked_interpreter
{
    struct stack_t
    {
        union
        {
            ::uwvm::wasm::wasm_i32 i32;
            ::uwvm::wasm::wasm_i64 i64;
            ::uwvm::wasm::wasm_f32 f32;
            ::uwvm::wasm::wasm_f64 f64;
            ::uwvm::wasm::wasm_v128 v128;
            ::std::size_t ref;
            ::uwvm::wasm::value_type null_reftype;
        };

    };

}  // namespace uwvm::vm::unchecked_interpreter
