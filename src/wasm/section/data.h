#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/data.h"

namespace uwvm::wasm::section
{
    struct data_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t count{};
        ::fast_io::vector<::uwvm::wasm::data_segment> entries{};
    };
}  // namespace uwvm::wasm::section

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::wasm::section::data_section>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::wasm::section::data_section>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
