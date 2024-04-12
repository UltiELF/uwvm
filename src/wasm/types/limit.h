#pragma once
#include <cstddef>
#include <cstdint>

namespace uwvm::wasm
{
    struct limit
    {
        ::std::size_t min{};
        ::std::size_t max{};
    };
}  // namespace uwvm::wasm::section
