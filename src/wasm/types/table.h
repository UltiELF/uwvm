#pragma once
#include <cstddef>
#include <cstdint>
#include "value.h"
#include "limit.h"

namespace uwvm::wasm
{
    struct table_type
    {
        ::uwvm::wasm::limit limit{};
        ::uwvm::wasm::value_type elem_type{}; // ref
        bool is_shared{};
    };
}
