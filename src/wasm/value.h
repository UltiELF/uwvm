#pragma once
#include <cstdint>
#include <fast_io_core_impl/terminate.h>

namespace uwvm::wasm
{
    enum value_type : ::std::uint_least8_t
    {
        none,
        any,
        i32,
        i64,
        f32,
        f64,
        v128,
        externref,
        funcref
    };

    inline constexpr ::std::uint_least8_t get_type_byte_width(value_type type) noexcept
    {
        switch(type)
        {
            case value_type::i32: return 4;
            case value_type::i64: return 8;
            case value_type::f32: return 4;
            case value_type::f64: return 8;
            case value_type::v128: return 16;
            case value_type::externref: [[fallthrough]];
            case value_type::funcref: return sizeof(void*);
            default: ::fast_io::unreachable();
        }
    }
}  // namespace uwvm::wasm
