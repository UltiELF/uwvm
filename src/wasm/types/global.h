#pragma once
#include "value.h"

namespace uwvm::wasm
{
    struct global_type
    {
        ::uwvm::wasm::value_type type;
        bool is_mutable;
    };
}  // namespace uwvm::wasm
