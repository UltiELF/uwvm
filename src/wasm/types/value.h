#pragma once
#include <cstdint>
#include <concepts>
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

    inline constexpr bool is_valid_value_type_value_with_v128_ref(value_type type) noexcept
    {
        switch(type)
        {
            case value_type::i32: [[fallthrough]];
            case value_type::i64: [[fallthrough]];
            case value_type::f32: [[fallthrough]];
            case value_type::f64: [[fallthrough]];
            case value_type::v128: [[fallthrough]];
            case value_type::funcref: [[fallthrough]];
            case value_type::externref: return true;
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

    template <::std::integral char_type>
    inline constexpr auto get_value_name(value_type valtype) noexcept
    {
        if constexpr(::std::same_as<char_type, char>)
        {
            switch(valtype)
            {
                case value_type::none: return ::fast_io::string_view{"none"};
                case value_type::i32: return ::fast_io::string_view{"i32"};
                case value_type::i64: return ::fast_io::string_view{"i64"};
                case value_type::f32: return ::fast_io::string_view{"f32"};
                case value_type::f64: return ::fast_io::string_view{"f64"};
                case value_type::v128: return ::fast_io::string_view{"v128"};
                case value_type::funcref: return ::fast_io::string_view{"funcref"};
                case value_type::externref: return ::fast_io::string_view{"externref"};
                case value_type::functype: return ::fast_io::string_view{"functype"};
                case value_type::resulttype: return ::fast_io::string_view{"resulttype"};
                default: return ::fast_io::string_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, wchar_t>)
        {
            switch(valtype)
            {
                case value_type::none: return ::fast_io::wstring_view{L"none"};
                case value_type::i32: return ::fast_io::wstring_view{L"i32"};
                case value_type::i64: return ::fast_io::wstring_view{L"i64"};
                case value_type::f32: return ::fast_io::wstring_view{L"f32"};
                case value_type::f64: return ::fast_io::wstring_view{L"f64"};
                case value_type::v128: return ::fast_io::wstring_view{L"v128"};
                case value_type::funcref: return ::fast_io::wstring_view{L"funcref"};
                case value_type::externref: return ::fast_io::wstring_view{L"externref"};
                case value_type::functype: return ::fast_io::wstring_view{L"functype"};
                case value_type::resulttype: return ::fast_io::wstring_view{L"resulttype"};
                default: return ::fast_io::wstring_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, char8_t>)
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
                default: return ::fast_io::u8string_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, char16_t>)
        {
            switch(valtype)
            {
                case value_type::none: return ::fast_io::u16string_view{u"none"};
                case value_type::i32: return ::fast_io::u16string_view{u"i32"};
                case value_type::i64: return ::fast_io::u16string_view{u"i64"};
                case value_type::f32: return ::fast_io::u16string_view{u"f32"};
                case value_type::f64: return ::fast_io::u16string_view{u"f64"};
                case value_type::v128: return ::fast_io::u16string_view{u"v128"};
                case value_type::funcref: return ::fast_io::u16string_view{u"funcref"};
                case value_type::externref: return ::fast_io::u16string_view{u"externref"};
                case value_type::functype: return ::fast_io::u16string_view{u"functype"};
                case value_type::resulttype: return ::fast_io::u16string_view{u"resulttype"};
                default: return ::fast_io::u16string_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, char32_t>)
        {
            switch(valtype)
            {
                case value_type::none: return ::fast_io::u32string_view{U"none"};
                case value_type::i32: return ::fast_io::u32string_view{U"i32"};
                case value_type::i64: return ::fast_io::u32string_view{U"i64"};
                case value_type::f32: return ::fast_io::u32string_view{U"f32"};
                case value_type::f64: return ::fast_io::u32string_view{U"f64"};
                case value_type::v128: return ::fast_io::u32string_view{U"v128"};
                case value_type::funcref: return ::fast_io::u32string_view{U"funcref"};
                case value_type::externref: return ::fast_io::u32string_view{U"externref"};
                case value_type::functype: return ::fast_io::u32string_view{U"functype"};
                case value_type::resulttype: return ::fast_io::u32string_view{U"resulttype"};
                default: return ::fast_io::u32string_view{};
            }
        }
    }

    template <::std::integral char_type>
    inline constexpr ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, value_type>) noexcept
    {
        return 10u;
    }

    namespace details
    {
        template <::std::integral char_type>
        inline constexpr char_type* print_reserve_value_type_impl(char_type* iter, value_type valtype) noexcept
        {
            if constexpr(::std::same_as<char_type, char>)
            {
                switch(valtype)
                {
                    case value_type::none: return ::fast_io::freestanding::my_copy_n(u"none", 4u, iter);
                    case value_type::i32: return ::fast_io::freestanding::my_copy_n(u"i32", 3u, iter);
                    case value_type::i64: return ::fast_io::freestanding::my_copy_n(u"i64", 3u, iter);
                    case value_type::f32: return ::fast_io::freestanding::my_copy_n(u"f32", 3u, iter);
                    case value_type::f64: return ::fast_io::freestanding::my_copy_n(u"f64", 3u, iter);
                    case value_type::v128: return ::fast_io::freestanding::my_copy_n(u"v128", 4u, iter);
                    case value_type::funcref: return ::fast_io::freestanding::my_copy_n(u"funcref", 7u, iter);
                    case value_type::externref: return ::fast_io::freestanding::my_copy_n(u"externref", 9u, iter);
                    case value_type::functype: return ::fast_io::freestanding::my_copy_n(u"functype", 8u, iter);
                    case value_type::resulttype: return ::fast_io::freestanding::my_copy_n(u"resulttype", 10u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, wchar_t>)
            {
                switch(valtype)
                {
                    case value_type::none: return ::fast_io::freestanding::my_copy_n(L"none", 4u, iter);
                    case value_type::i32: return ::fast_io::freestanding::my_copy_n(L"i32", 3u, iter);
                    case value_type::i64: return ::fast_io::freestanding::my_copy_n(L"i64", 3u, iter);
                    case value_type::f32: return ::fast_io::freestanding::my_copy_n(L"f32", 3u, iter);
                    case value_type::f64: return ::fast_io::freestanding::my_copy_n(L"f64", 3u, iter);
                    case value_type::v128: return ::fast_io::freestanding::my_copy_n(L"v128", 4u, iter);
                    case value_type::funcref: return ::fast_io::freestanding::my_copy_n(L"funcref", 7u, iter);
                    case value_type::externref: return ::fast_io::freestanding::my_copy_n(L"externref", 9u, iter);
                    case value_type::functype: return ::fast_io::freestanding::my_copy_n(L"functype", 8u, iter);
                    case value_type::resulttype: return ::fast_io::freestanding::my_copy_n(L"resulttype", 10u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char8_t>)
            {
                switch(valtype)
                {
                    case value_type::none: return ::fast_io::freestanding::my_copy_n(u8"none", 4u, iter);
                    case value_type::i32: return ::fast_io::freestanding::my_copy_n(u8"i32", 3u, iter);
                    case value_type::i64: return ::fast_io::freestanding::my_copy_n(u8"i64", 3u, iter);
                    case value_type::f32: return ::fast_io::freestanding::my_copy_n(u8"f32", 3u, iter);
                    case value_type::f64: return ::fast_io::freestanding::my_copy_n(u8"f64", 3u, iter);
                    case value_type::v128: return ::fast_io::freestanding::my_copy_n(u8"v128", 4u, iter);
                    case value_type::funcref: return ::fast_io::freestanding::my_copy_n(u8"funcref", 7u, iter);
                    case value_type::externref: return ::fast_io::freestanding::my_copy_n(u8"externref", 9u, iter);
                    case value_type::functype: return ::fast_io::freestanding::my_copy_n(u8"functype", 8u, iter);
                    case value_type::resulttype: return ::fast_io::freestanding::my_copy_n(u8"resulttype", 10u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char16_t>)
            {
                switch(valtype)
                {
                    case value_type::none: return ::fast_io::freestanding::my_copy_n(u"none", 4u, iter);
                    case value_type::i32: return ::fast_io::freestanding::my_copy_n(u"i32", 3u, iter);
                    case value_type::i64: return ::fast_io::freestanding::my_copy_n(u"i64", 3u, iter);
                    case value_type::f32: return ::fast_io::freestanding::my_copy_n(u"f32", 3u, iter);
                    case value_type::f64: return ::fast_io::freestanding::my_copy_n(u"f64", 3u, iter);
                    case value_type::v128: return ::fast_io::freestanding::my_copy_n(u"v128", 4u, iter);
                    case value_type::funcref: return ::fast_io::freestanding::my_copy_n(u"funcref", 7u, iter);
                    case value_type::externref: return ::fast_io::freestanding::my_copy_n(u"externref", 9u, iter);
                    case value_type::functype: return ::fast_io::freestanding::my_copy_n(u"functype", 8u, iter);
                    case value_type::resulttype: return ::fast_io::freestanding::my_copy_n(u"resulttype", 10u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char32_t>)
            {
                switch(valtype)
                {
                    case value_type::none: return ::fast_io::freestanding::my_copy_n(U"none", 4u, iter);
                    case value_type::i32: return ::fast_io::freestanding::my_copy_n(U"i32", 3u, iter);
                    case value_type::i64: return ::fast_io::freestanding::my_copy_n(U"i64", 3u, iter);
                    case value_type::f32: return ::fast_io::freestanding::my_copy_n(U"f32", 3u, iter);
                    case value_type::f64: return ::fast_io::freestanding::my_copy_n(U"f64", 3u, iter);
                    case value_type::v128: return ::fast_io::freestanding::my_copy_n(U"v128", 4u, iter);
                    case value_type::funcref: return ::fast_io::freestanding::my_copy_n(U"funcref", 7u, iter);
                    case value_type::externref: return ::fast_io::freestanding::my_copy_n(U"externref", 9u, iter);
                    case value_type::functype: return ::fast_io::freestanding::my_copy_n(U"functype", 8u, iter);
                    case value_type::resulttype: return ::fast_io::freestanding::my_copy_n(U"resulttype", 10u, iter);
                    default: return iter;
                }
            }
        }
    }  // namespace details

    template <::std::integral char_type>
    inline constexpr char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, value_type>, char_type* iter, value_type valtype) noexcept
    {
        return details::print_reserve_value_type_impl(iter, valtype);
    }

}  // namespace uwvm::wasm
