#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/function.h"

namespace uwvm::wasm::section
{

    struct function_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t function_count{};
        ::fast_io::vector<local_function_type> types{};
    };
}  // namespace uwvm::wasm::section

namespace fast_io::freestanding
{

    struct is_trivially_relocatable<::uwvm::wasm::section::function_section>
    {
        inline static constexpr bool value = true;
    };

    struct is_zero_default_constructible<::uwvm::wasm::section::function_section>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
