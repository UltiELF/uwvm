#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/function.h"

namespace uwvm::wasm::section
{
    struct type_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t type_count{};
        ::fast_io::vector<::uwvm::wasm::function_type> types{};
    };
}  // namespace uwvm::wasm::section
