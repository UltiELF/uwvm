#pragma once
#include <cstddef>
#include <cstdint>

namespace uwvm::wasm
{
    struct limits
    {
        ::std::size_t min{};
        ::std::size_t max{};
        bool present_max{};
    };
}  // namespace uwvm::wasm::section
