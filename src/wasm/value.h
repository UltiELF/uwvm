#pragma once
#include <cstdint>
#include <fast_io_core_impl/terminate.h>

namespace uwvm::wasm
{
    enum class value_type : ::std::uint_fast8_t
    {
        none = 0x00,
        i32 = 0x7F,
        i64 = 0x7E,
        f32 = 0x7D,
        f64 = 0x7C,
        v128 = 0x7B,
        funcref = 0x70,
        externref = 0x6F,
        functype = 0x60,
        resulttype = 0x40,
    };

    inline constexpr ::std::uint_fast8_t get_type_byte_width(value_type type) noexcept
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
            default: return 0;
        }
    }
}  // namespace uwvm::wasm
