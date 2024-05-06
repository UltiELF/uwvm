#pragma once
#include <cstddef>
#include <cstdint>

namespace uwvm::wasm::section
{
    struct start_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t index{};
    };
}  // namespace uwvm::wasm::section
