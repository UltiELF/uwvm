﻿#pragma once
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
            case uwvm::wasm::value_type::none: [[fall_through]];
            case uwvm::wasm::value_type::i32: [[fall_through]];
            case uwvm::wasm::value_type::i64: [[fall_through]];
            case uwvm::wasm::value_type::f32: [[fall_through]];
            case uwvm::wasm::value_type::f64: [[fall_through]];
            case uwvm::wasm::value_type::v128: [[fall_through]];
            case uwvm::wasm::value_type::funcref: [[fall_through]];
            case uwvm::wasm::value_type::externref: [[fall_through]];
            case uwvm::wasm::value_type::functype: [[fall_through]];
            case uwvm::wasm::value_type::resulttype: return true;
            default: return false;
        }
    }

    inline constexpr ::fast_io::u8string_view get_value_u8name(value_type valtype) noexcept
    {
        switch(valtype)
        {
            case uwvm::wasm::value_type::none: ::fast_io::u8string_view{u8"nil"};
            case uwvm::wasm::value_type::i32: ::fast_io::u8string_view{u8"i32"};
            case uwvm::wasm::value_type::i64: ::fast_io::u8string_view{u8"i64"};
            case uwvm::wasm::value_type::f32: ::fast_io::u8string_view{u8"f32"};
            case uwvm::wasm::value_type::f64: ::fast_io::u8string_view{u8"f64"};
            case uwvm::wasm::value_type::v128: ::fast_io::u8string_view{u8"v128"};
            case uwvm::wasm::value_type::funcref: ::fast_io::u8string_view{u8"funcref"};
            case uwvm::wasm::value_type::externref: ::fast_io::u8string_view{u8"externref"};
            case uwvm::wasm::value_type::functype: ::fast_io::u8string_view{u8"functype"};
            case uwvm::wasm::value_type::resulttype: ::fast_io::u8string_view{u8"resulttype"};
            default: return {};
        }
    }

}  // namespace uwvm::wasm
