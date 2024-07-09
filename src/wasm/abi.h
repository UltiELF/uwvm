#pragma once
#include <cstdint>
#include <concepts>
#include <fast_io_dsal/string_view.h>

namespace uwvm::wasm
{
    enum class abi : ::std::uint_fast32_t
    {
        detect,
        bare,
        emscripten,
        wasi,
    };

    template <::std::integral char_type>
    inline constexpr auto get_abi_name(abi abitype) noexcept
    {
        if constexpr(::std::same_as<char_type, char>)
        {
            switch(abitype)
            {
                case abi::detect: return ::fast_io::string_view{"detect"};
                case abi::bare: return ::fast_io::string_view{"bare"};
                case abi::emscripten: return ::fast_io::string_view{"emscripten"};
                case abi::wasi: return ::fast_io::string_view{"wasi"};
                default: return ::fast_io::string_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, wchar_t>)
        {
            switch(abitype)
            {
                case abi::detect: return ::fast_io::wstring_view{L"detect"};
                case abi::bare: return ::fast_io::wstring_view{L"bare"};
                case abi::emscripten: return ::fast_io::wstring_view{L"emscripten"};
                case abi::wasi: return ::fast_io::wstring_view{L"wasi"};
                default: return ::fast_io::wstring_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, char8_t>)
        {
            switch(abitype)
            {
                case abi::detect: return ::fast_io::u8string_view{u8"detect"};
                case abi::bare: return ::fast_io::u8string_view{u8"bare"};
                case abi::emscripten: return ::fast_io::u8string_view{u8"emscripten"};
                case abi::wasi: return ::fast_io::u8string_view{u8"wasi"};
                default: return ::fast_io::u8string_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, char16_t>)
        {
            switch(abitype)
            {
                case abi::detect: return ::fast_io::u16string_view{u"detect"};
                case abi::bare: return ::fast_io::u16string_view{u"bare"};
                case abi::emscripten: return ::fast_io::u16string_view{u"emscripten"};
                case abi::wasi: return ::fast_io::u16string_view{u"wasi"};
                default: return ::fast_io::u16string_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, char32_t>)
        {
            switch(abitype)
            {
                case abi::detect: return ::fast_io::u32string_view{U"detect"};
                case abi::bare: return ::fast_io::u32string_view{U"bare"};
                case abi::emscripten: return ::fast_io::u32string_view{U"emscripten"};
                case abi::wasi: return ::fast_io::u32string_view{U"wasi"};
                default: return ::fast_io::u32string_view{};
            }
        }
    }

    template <::std::integral char_type>
    inline constexpr ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, abi>) noexcept
    {
        return 10u;
    }

    namespace details
    {
        template <::std::integral char_type>
        inline constexpr char_type* print_reserve_abi_impl(char_type* iter, abi abitype) noexcept
        {
            if constexpr(::std::same_as<char_type, char>)
            {
                switch(abitype)
                {
                    case abi::detect: return ::fast_io::freestanding::my_copy_n("detect", 6u, iter);
                    case abi::bare: return ::fast_io::freestanding::my_copy_n("bare", 4u, iter);
                    case abi::emscripten: return ::fast_io::freestanding::my_copy_n("emscripten", 10u, iter);
                    case abi::wasi: return ::fast_io::freestanding::my_copy_n("wasi", 6u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, wchar_t>)
            {
                switch(abitype)
                {
                    case abi::detect: return ::fast_io::freestanding::my_copy_n(L"detect", 6u, iter);
                    case abi::bare: return ::fast_io::freestanding::my_copy_n(L"bare", 4u, iter);
                    case abi::emscripten: return ::fast_io::freestanding::my_copy_n(L"emscripten", 10u, iter);
                    case abi::wasi: return ::fast_io::freestanding::my_copy_n(L"wasi", 6u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char8_t>)
            {
                switch(abitype)
                {
                    case abi::detect: return ::fast_io::freestanding::my_copy_n(u8"detect", 6u, iter);
                    case abi::bare: return ::fast_io::freestanding::my_copy_n(u8"bare", 4u, iter);
                    case abi::emscripten: return ::fast_io::freestanding::my_copy_n(u8"emscripten", 10u, iter);
                    case abi::wasi: return ::fast_io::freestanding::my_copy_n(u8"wasi", 6u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char16_t>)
            {
                switch(abitype)
                {
                    case abi::detect: return ::fast_io::freestanding::my_copy_n(u"detect", 6u, iter);
                    case abi::bare: return ::fast_io::freestanding::my_copy_n(u"bare", 4u, iter);
                    case abi::emscripten: return ::fast_io::freestanding::my_copy_n(u"emscripten", 10u, iter);
                    case abi::wasi: return ::fast_io::freestanding::my_copy_n(u"wasi", 6u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char32_t>)
            {
                switch(abitype)
                {
                    case abi::detect: return ::fast_io::freestanding::my_copy_n(U"detect", 6u, iter);
                    case abi::bare: return ::fast_io::freestanding::my_copy_n(U"bare", 4u, iter);
                    case abi::emscripten: return ::fast_io::freestanding::my_copy_n(U"emscripten", 10u, iter);
                    case abi::wasi: return ::fast_io::freestanding::my_copy_n(U"wasi", 6u, iter);
                    default: return iter;
                }
            }
        }
    }  // namespace details

    template <::std::integral char_type>
    inline constexpr char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, abi>, char_type* iter, abi valtype) noexcept
    {
        return details::print_reserve_abi_impl(iter, valtype);
    }

}  // namespace uwvm::wasm
