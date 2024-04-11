#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../value.h"

namespace uwvm::wasm::section
{
    struct function_type
    {
        ::uwvm::wasm::value_type const* parameter_begin{};
        ::uwvm::wasm::value_type const* parameter_end{};

        ::uwvm::wasm::value_type const* result_begin{};
        ::uwvm::wasm::value_type const* result_end{};
    };

    struct type_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t type_count{};
        ::fast_io::vector<function_type> types{};
    };
}  // namespace uwvm::wasm::section
