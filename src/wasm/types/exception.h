#pragma once
#include "value.h"

namespace uwvm::wasm
{
    struct exception_type
    {
        ::uwvm::wasm::value_type const* parameter_begin{};
        ::uwvm::wasm::value_type const* parameter_end{};
    };
}  // namespace uwvm::wasm
