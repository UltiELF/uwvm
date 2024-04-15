#pragma once
#include <cstdint>
#include <concepts>
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
        tag = 4U  // exception
    };

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

    template <::std::integral char_type>
    inline constexpr auto get_extern_kind_name(extern_kind exttype) noexcept
    {
        if constexpr(::std::same_as<char_type, char>)
        {
            switch(exttype)
            {
                case extern_kind::func: return ::fast_io::string_view{"func"};
                case extern_kind::table: return ::fast_io::string_view{"table"};
                case extern_kind::memory: return ::fast_io::string_view{"memory"};
                case extern_kind::global: return ::fast_io::string_view{"global"};
                case extern_kind::tag: return ::fast_io::string_view{"tag"};
                default: return {};
            }
        }
        else if constexpr(::std::same_as<char_type, wchar_t>)
        {
            switch(exttype)
            {
                case extern_kind::func: return ::fast_io::wstring_view{L"func"};
                case extern_kind::table: return ::fast_io::wstring_view{L"table"};
                case extern_kind::memory: return ::fast_io::wstring_view{L"memory"};
                case extern_kind::global: return ::fast_io::wstring_view{L"global"};
                case extern_kind::tag: return ::fast_io::wstring_view{L"tag"};
                default: return {};
            }
        }
        else if constexpr(::std::same_as<char_type, char8_t>)
        {
            switch(exttype)
            {
                case extern_kind::func: return ::fast_io::u8string_view{u8"func"};
                case extern_kind::table: return ::fast_io::u8string_view{u8"table"};
                case extern_kind::memory: return ::fast_io::u8string_view{u8"memory"};
                case extern_kind::global: return ::fast_io::u8string_view{u8"global"};
                case extern_kind::tag: return ::fast_io::u8string_view{u8"tag"};
                default: return {};
            }
        }
        else if constexpr(::std::same_as<char_type, char16_t>)
        {
            switch(exttype)
            {
                case extern_kind::func: return ::fast_io::u16string_view{u"func"};
                case extern_kind::table: return ::fast_io::u16string_view{u"table"};
                case extern_kind::memory: return ::fast_io::u16string_view{u"memory"};
                case extern_kind::global: return ::fast_io::u16string_view{u"global"};
                case extern_kind::tag: return ::fast_io::u16string_view{u"tag"};
                default: return {};
            }
        }
        else if constexpr(::std::same_as<char_type, char32_t>)
        {
            switch(exttype)
            {
                case extern_kind::func: return ::fast_io::u32string_view{U"func"};
                case extern_kind::table: return ::fast_io::u32string_view{U"table"};
                case extern_kind::memory: return ::fast_io::u32string_view{U"memory"};
                case extern_kind::global: return ::fast_io::u32string_view{U"global"};
                case extern_kind::tag: return ::fast_io::u32string_view{U"tag"};
                default: return {};
            }
        }
    }

    template <::std::integral char_type>
    inline constexpr ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, extern_kind>) noexcept
    {
        return 6u;
    }

    namespace details
    {
        template <::std::integral char_type>
        inline constexpr char_type* print_reserve_extern_kind_impl(char_type* iter, extern_kind exttype) noexcept
        {
            if constexpr(::std::same_as<char_type, char>)
            {
                switch(exttype)
                {
                    case extern_kind::func: return ::fast_io::freestanding::my_copy_n("func", 4u, iter);
                    case extern_kind::table: return ::fast_io::freestanding::my_copy_n("table", 5u, iter);
                    case extern_kind::memory: return ::fast_io::freestanding::my_copy_n("memory", 6u, iter);
                    case extern_kind::global: return ::fast_io::freestanding::my_copy_n("global", 6u, iter);
                    case extern_kind::tag: return ::fast_io::freestanding::my_copy_n("tag", 3u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, wchar_t>)
            {
                switch(exttype)
                {
                    case extern_kind::func: return ::fast_io::freestanding::my_copy_n(L"func", 4u, iter);
                    case extern_kind::table: return ::fast_io::freestanding::my_copy_n(L"table", 5u, iter);
                    case extern_kind::memory: return ::fast_io::freestanding::my_copy_n(L"memory", 6u, iter);
                    case extern_kind::global: return ::fast_io::freestanding::my_copy_n(L"global", 6u, iter);
                    case extern_kind::tag: return ::fast_io::freestanding::my_copy_n(L"tag", 3u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char8_t>)
            {
                switch(exttype)
                {
                    case extern_kind::func: return ::fast_io::freestanding::my_copy_n(u8"func", 4u, iter);
                    case extern_kind::table: return ::fast_io::freestanding::my_copy_n(u8"table", 5u, iter);
                    case extern_kind::memory: return ::fast_io::freestanding::my_copy_n(u8"memory", 6u, iter);
                    case extern_kind::global: return ::fast_io::freestanding::my_copy_n(u8"global", 6u, iter);
                    case extern_kind::tag: return ::fast_io::freestanding::my_copy_n(u8"tag", 3u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char16_t>)
            {
                switch(exttype)
                {
                    case extern_kind::func: return ::fast_io::freestanding::my_copy_n(u"func", 4u, iter);
                    case extern_kind::table: return ::fast_io::freestanding::my_copy_n(u"table", 5u, iter);
                    case extern_kind::memory: return ::fast_io::freestanding::my_copy_n(u"memory", 6u, iter);
                    case extern_kind::global: return ::fast_io::freestanding::my_copy_n(u"global", 6u, iter);
                    case extern_kind::tag: return ::fast_io::freestanding::my_copy_n(u"tag", 3u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char32_t>)
            {
                switch(exttype)
                {
                    case extern_kind::func: return ::fast_io::freestanding::my_copy_n(U"func", 4u, iter);
                    case extern_kind::table: return ::fast_io::freestanding::my_copy_n(U"table", 5u, iter);
                    case extern_kind::memory: return ::fast_io::freestanding::my_copy_n(U"memory", 6u, iter);
                    case extern_kind::global: return ::fast_io::freestanding::my_copy_n(U"global", 6u, iter);
                    case extern_kind::tag: return ::fast_io::freestanding::my_copy_n(U"tag", 3u, iter);
                    default: return iter;
                }
            }
        }
    }  // namespace details

    template <::std::integral char_type>
    inline constexpr char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, extern_kind>, char_type* iter, extern_kind exttype) noexcept
    {
        return details::print_reserve_extern_kind_impl(iter, exttype);
    }

}  // namespace uwvm::wasm
