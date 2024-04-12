#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>
#include "type.h"
#include "../types.h"

namespace uwvm::wasm::section
{
    struct import_type
    {
        char8_t const* module_begin{};
        char8_t const* module_end{};

        char8_t const* name_begin{};
        char8_t const* name_end{};

        ::uwvm::wasm::extern_type extern_type{};
    };

    struct import_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t import_count{};
        ::fast_io::vector<import_type> types{};
        ::fast_io::vector<import_type const*> func_types{};
        ::fast_io::vector<import_type const*> table_types{};
        ::fast_io::vector<import_type const*> memory_types{};
        ::fast_io::vector<import_type const*> global_types{};
        ::fast_io::vector<import_type const*> tag_types{};
    };
}  // namespace uwvm::wasm::section
