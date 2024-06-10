#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/element.h"

namespace uwvm::wasm::section
{
    struct element_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t elem_segment_count{};
        ::fast_io::vector<::uwvm::wasm::elem_segment_type> elem_segments{};
    };
}  // namespace uwvm::wasm::section

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::wasm::section::element_section>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::wasm::section::element_section>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
