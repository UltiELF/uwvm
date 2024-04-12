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

    inline constexpr bool is_valid_value_type(value_type type) noexcept
    {
        switch(type)
        {
            case value_type::none: [[fallthrough]];
            case value_type::i32: [[fallthrough]];
            case value_type::i64: [[fallthrough]];
            case value_type::f32: [[fallthrough]];
            case value_type::f64: [[fallthrough]];
            case value_type::v128: [[fallthrough]];
            case value_type::funcref: [[fallthrough]];
            case value_type::externref: [[fallthrough]];
            case value_type::functype: [[fallthrough]];
            case value_type::resulttype: return true;
            default: return false;
        }
    }

    inline constexpr bool is_valid_value_type_value(value_type type) noexcept
    {
        switch(type)
        {
            case value_type::i32: [[fallthrough]];
            case value_type::i64: [[fallthrough]];
            case value_type::f32: [[fallthrough]];
            case value_type::f64: return true;
            default: return false;
        }
    }

    inline constexpr bool is_valid_value_type_value_with_v128(value_type type) noexcept
    {
        switch(type)
        {
            case value_type::i32: [[fallthrough]];
            case value_type::i64: [[fallthrough]];
            case value_type::f32: [[fallthrough]];
            case value_type::f64: [[fallthrough]];
            case value_type::v128: return true;
            default: return false;
        }
    }

    inline constexpr bool is_valid_value_type_ref(value_type type) noexcept
    {
        switch(type)
        {
            case value_type::funcref: [[fallthrough]];
            case value_type::externref: return true;
            default: return false;
        }
    }

    inline constexpr ::fast_io::u8string_view get_value_u8name(value_type valtype) noexcept
    {
        switch(valtype)
        {
            case value_type::none: return ::fast_io::u8string_view{u8"none"};
            case value_type::i32: return ::fast_io::u8string_view{u8"i32"};
            case value_type::i64: return ::fast_io::u8string_view{u8"i64"};
            case value_type::f32: return ::fast_io::u8string_view{u8"f32"};
            case value_type::f64: return ::fast_io::u8string_view{u8"f64"};
            case value_type::v128: return ::fast_io::u8string_view{u8"v128"};
            case value_type::funcref: return ::fast_io::u8string_view{u8"funcref"};
            case value_type::externref: return ::fast_io::u8string_view{u8"externref"};
            case value_type::functype: return ::fast_io::u8string_view{u8"functype"};
            case value_type::resulttype: return ::fast_io::u8string_view{u8"resulttype"};
            default: return {};
        }
    }

}  // namespace uwvm::wasm
