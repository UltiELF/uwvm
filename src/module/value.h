#pragma once
#include <cstdint>
#include <fast_io_core_impl/terminate.h>

namespace uwvm
{
    enum value_type : ::std::uint_least8_t
    {
        none,
        any,
        i32,
        i64,
        u32,
        u64,
        f32,
        f64,
        v128,
        externref,
        funcref
    };

    constexpr inline ::std::uint_least8_t get_type_byte_width(value_type type) noexcept
    {
        switch(type)
        {
            case uwvm::i32: return 4;
            case uwvm::i64: return 8;
            case uwvm::u32: return 4;
            case uwvm::u64: return 8;
            case uwvm::f32: return 4;
            case uwvm::f64: return 8;
            case uwvm::v128: return 16;
            case uwvm::externref: [[fallthrough]];
            case uwvm::funcref: return sizeof(void*);
            default: ::fast_io::unreachable();
        }
    }
}  // namespace uwvm
