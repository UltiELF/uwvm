#pragma once 
#include <cstddef>
#include "limit.h"

namespace uwvm::wasm
{
    struct memory_type
    {
        ::uwvm::wasm::limit mem_limit{0, SIZE_MAX};
        bool is_shared{};
        bool is_index64{};
    };
}  // namespace uwvm::wasm::section
