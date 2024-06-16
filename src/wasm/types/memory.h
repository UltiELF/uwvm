#pragma once 
#include <cstddef>
#include "limit.h"

namespace uwvm::wasm
{
    struct memory_type
    {
        ::uwvm::wasm::limits limits{};

        ::std::make_signed_t<::std::size_t> idxtype{};
        ::std::size_t mempagesize{65536};
        bool is_shared{};  // thread
    };
}  // namespace uwvm::wasm::section
