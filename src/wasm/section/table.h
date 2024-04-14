#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/table.h"

namespace uwvm::wasm::section
{
    struct table_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t table_count{};
        ::fast_io::vector<table_type> types{};
    };
}  // namespace uwvm::wasm::section
