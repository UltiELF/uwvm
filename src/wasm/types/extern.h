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
        func = 0U,
        table = 1U,
        memory = 2U,
        global = 3U,
        tag = 4U // exception
    };

    inline constexpr ::fast_io::u8string_view get_extern_kind_u8name(extern_kind type) noexcept
    {
        switch(type)
        {
            case extern_kind::func: return ::fast_io::u8string_view{u8"func"};
            case extern_kind::table: return ::fast_io::u8string_view{u8"table"};
            case extern_kind::memory: return ::fast_io::u8string_view{u8"memory"};
            case extern_kind::global: return ::fast_io::u8string_view{u8"global"};
            case extern_kind::tag: return ::fast_io::u8string_view{u8"tag"};
            default: return ::fast_io::u8string_view{};
        }
    }

    inline constexpr bool is_valid_extern_kind_type(extern_kind type) noexcept
    {
        switch(type)
        {
            case extern_kind::func: [[fallthrough]];
            case extern_kind::table: [[fallthrough]];
            case extern_kind::memory: [[fallthrough]];
            case extern_kind::global: [[fallthrough]];
            case extern_kind::tag: return true;
            default: return false;
        }
    }

    struct extern_type
    {
        union
        {
            ::uwvm::wasm::function_type const* function;
            ::uwvm::wasm::table_type table;
            ::uwvm::wasm::memory_type memory;
            ::uwvm::wasm::global_type global;
            ::uwvm::wasm::exception_type exception;
        };

        extern_kind type{};
    };
}  // namespace uwvm::wasm
