#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/table.h"

namespace uwvm::wasm::section
{
    struct table_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t table_count{};
        ::fast_io::vector<table_type> types{};
    };
}  // namespace uwvm::wasm::section

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::wasm::section::table_section>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::wasm::section::table_section>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
