#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/memory.h"

namespace uwvm::wasm::section
{
    struct memory_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t memory_count{};
        ::fast_io::vector<memory_type> types{};
    };
}  // namespace uwvm::wasm::section

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::wasm::section::memory_section>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::wasm::section::memory_section>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
