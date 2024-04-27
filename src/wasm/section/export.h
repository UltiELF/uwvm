#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>

#include "../types/export.h"

namespace uwvm::wasm::section
{
    struct export_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t export_count{};

        ::fast_io::vector<::uwvm::wasm::export_type> types{};
        ::fast_io::vector<export_type const*> func_types{};
        ::fast_io::vector<export_type const*> table_types{};
        ::fast_io::vector<export_type const*> memory_types{};
        ::fast_io::vector<export_type const*> global_types{};
        ::fast_io::vector<export_type const*> tag_types{};
    };
}  // namespace uwvm::wasm::section

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::wasm::section::export_section>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::wasm::section::export_section>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
