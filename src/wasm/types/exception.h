#pragma once
#include <cstdint>
#include "value.h"
#include "function.h"

namespace uwvm::wasm
{
    struct exception_type
    {
        ::uwvm::wasm::function_type const* type{};
        ::std::uint_least8_t attribute{};
    };
}  // namespace uwvm::wasm
