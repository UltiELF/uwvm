#pragma once
#include "types.h"
#include "section.h"

namespace uwvm::wasm
{
    struct wasm_module
    {
        ::std::uint_least32_t wasm_version{};

        ::uwvm::wasm::section::type_section typesec{};
        ::uwvm::wasm::section::function_section functionsec{};
        ::uwvm::wasm::section::import_section importsec{};
        ::uwvm::wasm::section::table_section tablesec{};
    };
}  // namespace uwvm::wasm

namespace fast_io::freestanding
{

    struct is_trivially_relocatable<::uwvm::wasm::wasm_module>
    {
        inline static constexpr bool value = true;
    };

    struct is_zero_default_constructible<::uwvm::wasm::wasm_module>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
