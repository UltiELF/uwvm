#pragma once
#include <cstddef>
#include <cstdint>

namespace uwvm::section
{
    struct type_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t count{};
    };
}  // namespace uwvm::section
