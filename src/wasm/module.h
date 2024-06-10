#pragma once
#include <fast_io_dsal/string_view.h>

#include "types.h"
#include "section.h"

namespace uwvm::wasm
{
    struct wasm_module
    {
        ::std::byte const* module_begin{};
        ::std::byte const* module_end{};

        ::fast_io::u8string_view module_name{};
        ::std::uint_least32_t wasm_version{};

        ::uwvm::wasm::section::type_section typesec{};
        ::uwvm::wasm::section::function_section functionsec{};
        ::uwvm::wasm::section::import_section importsec{};
        ::uwvm::wasm::section::table_section tablesec{};
        ::uwvm::wasm::section::memory_section memorysec{};
        ::uwvm::wasm::section::global_section globalsec{};
        ::uwvm::wasm::section::export_section exportsec{};
        ::uwvm::wasm::section::start_section startsec{};
        ::uwvm::wasm::section::element_section elemsec{};
        ::uwvm::wasm::section::code_section codesec{};
        ::uwvm::wasm::section::data_section datasec{};
    };
}  // namespace uwvm::wasm

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::wasm::wasm_module>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::wasm::wasm_module>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
