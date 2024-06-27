#pragma once
#include <cstddef>
#include <cstdint>
#include "value.h"
#include "initializer.h"

namespace uwvm::wasm
{
    struct data_segment
    {
        ::std::size_t index{};
        ::uwvm::wasm::initializer_exp offset{};
        ::std::size_t size{};
        ::std::byte const* data_begin{};
        ::std::byte const* data_end{};

    };


}  // namespace uwvm::wasm
