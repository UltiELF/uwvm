#pragma once
#include <cstddef>
#include <cstdint>
#include "extern.h"

namespace uwvm::wasm
{
    struct export_type
    {
        char8_t const* name_begin{};
        char8_t const* name_end{};

        ::uwvm::wasm::extern_kind kind{};
        ::std::size_t index{};
    };

}  // namespace uwvm::wasm::section
