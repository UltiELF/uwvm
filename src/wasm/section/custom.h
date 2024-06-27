#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

namespace uwvm::wasm::section
{
    struct custom_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        char8_t const* name_begin{};
        char8_t const* name_end{}; // custom begin
    };
}  // namespace uwvm::wasm::section
