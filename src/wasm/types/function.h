#pragma once
#include "value.h"

namespace uwvm::wasm
{
    struct function_type
    {
        ::uwvm::wasm::value_type const* parameter_begin{};
        ::uwvm::wasm::value_type const* parameter_end{};

        ::uwvm::wasm::value_type const* result_begin{};
        ::uwvm::wasm::value_type const* result_end{};
    };

    struct local_function_type
    {
        ::uwvm::wasm::function_type const* func_type{};
        ::std::byte const* code_begin{};
        ::std::byte const* code_end{};
        char8_t const* custom_name_begin{};
        char8_t const* custom_name_end{};
        bool is_shared{};
    };
}  // namespace uwvm::wasm
