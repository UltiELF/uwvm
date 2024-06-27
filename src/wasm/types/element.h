#pragma once
#include <cstdint>
#include <fast_io_dsal/vector.h>
#include "value.h"
#include "initializer.h"

namespace uwvm::wasm
{
    struct elem_segment_type
    {
        ::std::size_t index{};
        ::uwvm::wasm::initializer_exp initializer{};
        ::std::size_t elem_count{};
        ::fast_io::vector<::std::size_t> elems{};
    };
}  // namespace uwvm::wasm
