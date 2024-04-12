#pragma once 
#include <cstddef>
#include "limit.h"

namespace uwvm::wasm
{
    struct memory_type
    {
        ::uwvm::wasm::limit mem_limit{0, SIZE_MAX};
    };
}  // namespace uwvm::wasm::section
