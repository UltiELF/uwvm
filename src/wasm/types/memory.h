#pragma once 
#include <cstddef>
#include "limit.h"

namespace uwvm::wasm
{
    struct memory_type
    {
        ::uwvm::wasm::limit mem_limit{};
        bool is_index64{};
        bool is_shared{};  // export
    };
}  // namespace uwvm::wasm::section
