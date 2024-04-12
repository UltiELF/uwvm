#pragma once
#include <cstdint>
#include "value.h"
#include "function.h"
#include "table.h"
#include "memory.h"
#include "global.h"
#include "exception.h"

namespace uwvm::wasm
{
    enum class extern_kind : ::std::uint_fast8_t
    {
        invalid,
        function,
        table,
        memory,
        global,
        exception,
        tag = exception,
    };

    struct extern_type
    {
        union
        {
            ::uwvm::wasm::function_type function{};
            ::uwvm::wasm::table_type table;
            ::uwvm::wasm::memory_type memory;
            ::uwvm::wasm::global_type global;
            ::uwvm::wasm::exception_type exception;
        };
        extern_kind type{};
    }
}  // namespace uwvm::wasm
