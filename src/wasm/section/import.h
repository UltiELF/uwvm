#pragma once
#include <cstddef>
#include <cstdint>
#include <fast_io_dsal/vector.h>
#include "type.h"

namespace uwvm::wasm::section
{
    enum class import_basic_type : ::std::uint_fast8_t
    {
        func = 0U,
        table = 1U,
        memory = 2U,
        global = 3U
    };

    inline constexpr ::fast_io::u8string_view get_import_basic_u8name(import_basic_type type) noexcept
    {
        switch(type)
        {
            case uwvm::wasm::section::import_basic_type::func: return ::fast_io::u8string_view{u8"func"};
            case uwvm::wasm::section::import_basic_type::table: return ::fast_io::u8string_view{u8"table"};
            case uwvm::wasm::section::import_basic_type::memory: return ::fast_io::u8string_view{u8"memory"};
            case uwvm::wasm::section::import_basic_type::global: return ::fast_io::u8string_view{u8"global"};
            default: return ::fast_io::u8string_view{};
        }
    }

    inline constexpr bool is_valid_import_basic_type(import_basic_type type) noexcept
    {
        switch(type)
        {
            case uwvm::wasm::section::import_basic_type::func: [[fallthrough]];
            case uwvm::wasm::section::import_basic_type::table: [[fallthrough]];
            case uwvm::wasm::section::import_basic_type::memory: [[fallthrough]];
            case uwvm::wasm::section::import_basic_type::global: return true;
            default: return false;
        }
    }

    struct import_type
    {
        char8_t const* module_begin{};
        char8_t const* module_end{};

        char8_t const* name_begin{};
        char8_t const* name_end{};

        union
        {
            ::uwvm::wasm::function_type const* func_type{};
            // to do
        };
        import_basic_type type{};
    };

    struct import_section
    {
        ::std::byte const* sec_begin{};
        ::std::byte const* sec_end{};

        ::std::size_t import_count{};
        ::fast_io::vector<import_type> types{};
        ::fast_io::vector<import_type const*> func_types{};
        ::fast_io::vector<import_type const*> table_types{};
        ::fast_io::vector<import_type const*> memory_types{};
        ::fast_io::vector<import_type const*> global_types{};
    };
}  // namespace uwvm::wasm::section
