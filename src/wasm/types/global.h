#pragma once
#include "value.h"
#include "initializer.h"

namespace uwvm::wasm
{
    struct global_type
    {
        ::uwvm::wasm::value_type type{};
        bool is_mutable{};
        bool is_shared{};  // export
    };

    struct local_global_type
    {
        ::uwvm::wasm::initializer_exp initializer{};
        global_type type{};
    };

}  // namespace uwvm::wasm
