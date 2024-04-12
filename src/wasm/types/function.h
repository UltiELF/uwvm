#pragma once
#include "value.h"

namespace uwvm::wasm
{
    struct function_type
    {
        ::uwvm::wasm::value_type const* parameter_begin{};
        ::uwvm::wasm::value_type const* parameter_end{};

        ::uwvm::wasm::value_type const* result_begin{};
        ::uwvm::wasm::value_type const* result_end{};

    };
}  // namespace uwvm::wasm
