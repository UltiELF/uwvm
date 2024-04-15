#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/function.h"

namespace uwvm::wasm::section
{
    struct type_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t type_count{};
        ::fast_io::vector<::uwvm::wasm::function_type> types{};
    };
}  // namespace uwvm::wasm::section

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::wasm::section::type_section>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::wasm::section::type_section>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
