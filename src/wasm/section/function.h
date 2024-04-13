#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/function.h"

namespace uwvm::wasm::section
{



    struct function_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t function_count{};
        ::fast_io::vector<local_function_type> types{};
    };
}  // namespace uwvm::wasm::section
