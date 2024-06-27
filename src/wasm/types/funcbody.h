#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>
#include "value.h"

namespace uwvm::wasm
{
    struct local_entry
    {
        ::std::size_t count{};
        ::uwvm::wasm::value_type type{};
        char8_t const* custom_name_begin{};
        char8_t const* custom_name_end{};
    };

    struct func_body
    {
        ::std::size_t body_size{};
        ::std::size_t local_count{};
        ::fast_io::vector<local_entry> locals{};
        ::std::byte const* begin{};
        ::std::byte const* end{};
    };

}  // namespace uwvm::wasm
