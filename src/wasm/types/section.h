#pragma once
#include <cstdint>
#include <fast_io_dsal/string_view.h>

namespace uwvm::wasm
{
    enum class section_type : ::std::uint_fast8_t
    {
        custom_sec = 0U,
        type_sec,
        import_sec,
        function_sec,
        table_sec,
        memory_sec,
        global_sec,
        export_sec,
        start_sec,
        element_sec,
        code_sec,
        data_sec,
        data_count_sec,
        tag_sec
    };

    inline constexpr ::std::uint_fast8_t get_section_value(section_type sectype) noexcept { return static_cast<::std::uint_fast8_t>(sectype); }

    inline constexpr bool is_valid_section(section_type sectype) noexcept
    {
        return static_cast<::std::uint_fast8_t>(sectype) <= static_cast<::std::uint_fast8_t>(section_type::tag_sec);
    }

    template <::std::integral char_type>
    inline constexpr auto get_section_name(section_type sectype) noexcept
    {
        if constexpr(::std::same_as<char_type, char>)
        {
            switch(sectype)
            {
                case section_type::custom_sec: return ::fast_io::string_view{"Custom"};
                case section_type::type_sec: return ::fast_io::string_view{"Type"};
                case section_type::import_sec: return ::fast_io::string_view{"Import"};
                case section_type::function_sec: return ::fast_io::string_view{"Function"};
                case section_type::table_sec: return ::fast_io::string_view{"Table"};
                case section_type::memory_sec: return ::fast_io::string_view{"Memory"};
                case section_type::global_sec: return ::fast_io::string_view{"Global"};
                case section_type::export_sec: return ::fast_io::string_view{"Export"};
                case section_type::start_sec: return ::fast_io::string_view{"Start"};
                case section_type::element_sec: return ::fast_io::string_view{"Element"};
                case section_type::code_sec: return ::fast_io::string_view{"Code"};
                case section_type::data_sec: return ::fast_io::string_view{"Data"};
                case section_type::data_count_sec: return ::fast_io::string_view{"Data Count"};
                case section_type::tag_sec: return ::fast_io::string_view{"Tag"};
                default: return ::fast_io::string_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, wchar_t>)
        {
            switch(sectype)
            {
                case section_type::custom_sec: return ::fast_io::wstring_view{L"Custom"};
                case section_type::type_sec: return ::fast_io::wstring_view{L"Type"};
                case section_type::import_sec: return ::fast_io::wstring_view{L"Import"};
                case section_type::function_sec: return ::fast_io::wstring_view{L"Function"};
                case section_type::table_sec: return ::fast_io::wstring_view{L"Table"};
                case section_type::memory_sec: return ::fast_io::wstring_view{L"Memory"};
                case section_type::global_sec: return ::fast_io::wstring_view{L"Global"};
                case section_type::export_sec: return ::fast_io::wstring_view{L"Export"};
                case section_type::start_sec: return ::fast_io::wstring_view{L"Start"};
                case section_type::element_sec: return ::fast_io::wstring_view{L"Element"};
                case section_type::code_sec: return ::fast_io::wstring_view{L"Code"};
                case section_type::data_sec: return ::fast_io::wstring_view{L"Data"};
                case section_type::data_count_sec: return ::fast_io::wstring_view{L"Data Count"};
                case section_type::tag_sec: return ::fast_io::wstring_view{L"Tag"};
                default: return ::fast_io::wstring_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, char8_t>)
        {
            switch(sectype)
            {
                case section_type::custom_sec: return ::fast_io::u8string_view{u8"Custom"};
                case section_type::type_sec: return ::fast_io::u8string_view{u8"Type"};
                case section_type::import_sec: return ::fast_io::u8string_view{u8"Import"};
                case section_type::function_sec: return ::fast_io::u8string_view{u8"Function"};
                case section_type::table_sec: return ::fast_io::u8string_view{u8"Table"};
                case section_type::memory_sec: return ::fast_io::u8string_view{u8"Memory"};
                case section_type::global_sec: return ::fast_io::u8string_view{u8"Global"};
                case section_type::export_sec: return ::fast_io::u8string_view{u8"Export"};
                case section_type::start_sec: return ::fast_io::u8string_view{u8"Start"};
                case section_type::element_sec: return ::fast_io::u8string_view{u8"Element"};
                case section_type::code_sec: return ::fast_io::u8string_view{u8"Code"};
                case section_type::data_sec: return ::fast_io::u8string_view{u8"Data"};
                case section_type::data_count_sec: return ::fast_io::u8string_view{u8"Data Count"};
                case section_type::tag_sec: return ::fast_io::u8string_view{u8"Tag"};
                default: return ::fast_io::u8string_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, char16_t>)
        {
            switch(sectype)
            {
                case section_type::custom_sec: return ::fast_io::u16string_view{u"Custom"};
                case section_type::type_sec: return ::fast_io::u16string_view{u"Type"};
                case section_type::import_sec: return ::fast_io::u16string_view{u"Import"};
                case section_type::function_sec: return ::fast_io::u16string_view{u"Function"};
                case section_type::table_sec: return ::fast_io::u16string_view{u"Table"};
                case section_type::memory_sec: return ::fast_io::u16string_view{u"Memory"};
                case section_type::global_sec: return ::fast_io::u16string_view{u"Global"};
                case section_type::export_sec: return ::fast_io::u16string_view{u"Export"};
                case section_type::start_sec: return ::fast_io::u16string_view{u"Start"};
                case section_type::element_sec: return ::fast_io::u16string_view{u"Element"};
                case section_type::code_sec: return ::fast_io::u16string_view{u"Code"};
                case section_type::data_sec: return ::fast_io::u16string_view{u"Data"};
                case section_type::data_count_sec: return ::fast_io::u16string_view{u"Data Count"};
                case section_type::tag_sec: return ::fast_io::u16string_view{u"Tag"};
                default: return ::fast_io::u16string_view{};
            }
        }
        else if constexpr(::std::same_as<char_type, char32_t>)
        {
            switch(sectype)
            {
                case section_type::custom_sec: return ::fast_io::u32string_view{U"Custom"};
                case section_type::type_sec: return ::fast_io::u32string_view{U"Type"};
                case section_type::import_sec: return ::fast_io::u32string_view{U"Import"};
                case section_type::function_sec: return ::fast_io::u32string_view{U"Function"};
                case section_type::table_sec: return ::fast_io::u32string_view{U"Table"};
                case section_type::memory_sec: return ::fast_io::u32string_view{U"Memory"};
                case section_type::global_sec: return ::fast_io::u32string_view{U"Global"};
                case section_type::export_sec: return ::fast_io::u32string_view{U"Export"};
                case section_type::start_sec: return ::fast_io::u32string_view{U"Start"};
                case section_type::element_sec: return ::fast_io::u32string_view{U"Element"};
                case section_type::code_sec: return ::fast_io::u32string_view{U"Code"};
                case section_type::data_sec: return ::fast_io::u32string_view{U"Data"};
                case section_type::data_count_sec: return ::fast_io::u32string_view{U"Data Count"};
                case section_type::tag_sec: return ::fast_io::u32string_view{U"Tag"};
                default: return ::fast_io::u32string_view{};
            }
        }
    }

    template <::std::integral char_type>
    inline constexpr ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, section_type>) noexcept
    {
        return 10u;
    }

    namespace details
    {
        template <::std::integral char_type>
        inline constexpr char_type* print_section_type_type_impl(char_type* iter, section_type sectype) noexcept
        {
            if constexpr(::std::same_as<char_type, char>)
            {
                switch(sectype)
                {
                    case section_type::custom_sec: return ::fast_io::freestanding::my_copy_n("Custom", 6u, iter);
                    case section_type::type_sec: return ::fast_io::freestanding::my_copy_n("Type", 4u, iter);
                    case section_type::import_sec: return ::fast_io::freestanding::my_copy_n("Import", 6u, iter);
                    case section_type::function_sec: return ::fast_io::freestanding::my_copy_n("Function", 8u, iter);
                    case section_type::table_sec: return ::fast_io::freestanding::my_copy_n("Table", 5u, iter);
                    case section_type::memory_sec: return ::fast_io::freestanding::my_copy_n("Memory", 6u, iter);
                    case section_type::global_sec: return ::fast_io::freestanding::my_copy_n("Global", 6u, iter);
                    case section_type::export_sec: return ::fast_io::freestanding::my_copy_n("Export", 6u, iter);
                    case section_type::start_sec: return ::fast_io::freestanding::my_copy_n("Start", 5u, iter);
                    case section_type::element_sec: return ::fast_io::freestanding::my_copy_n("Element", 7u, iter);
                    case section_type::code_sec: return ::fast_io::freestanding::my_copy_n("Code", 4u, iter);
                    case section_type::data_sec: return ::fast_io::freestanding::my_copy_n("Data", 4u, iter);
                    case section_type::data_count_sec: return ::fast_io::freestanding::my_copy_n("Data Count", 10u, iter);
                    case section_type::tag_sec: return ::fast_io::freestanding::my_copy_n("Tag", 3u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, wchar_t>)
            {
                switch(sectype)
                {
                    case section_type::custom_sec: return ::fast_io::freestanding::my_copy_n(L"Custom", 6u, iter);
                    case section_type::type_sec: return ::fast_io::freestanding::my_copy_n(L"Type", 4u, iter);
                    case section_type::import_sec: return ::fast_io::freestanding::my_copy_n(L"Import", 6u, iter);
                    case section_type::function_sec: return ::fast_io::freestanding::my_copy_n(L"Function", 8u, iter);
                    case section_type::table_sec: return ::fast_io::freestanding::my_copy_n(L"Table", 5u, iter);
                    case section_type::memory_sec: return ::fast_io::freestanding::my_copy_n(L"Memory", 6u, iter);
                    case section_type::global_sec: return ::fast_io::freestanding::my_copy_n(L"Global", 6u, iter);
                    case section_type::export_sec: return ::fast_io::freestanding::my_copy_n(L"Export", 6u, iter);
                    case section_type::start_sec: return ::fast_io::freestanding::my_copy_n(L"Start", 5u, iter);
                    case section_type::element_sec: return ::fast_io::freestanding::my_copy_n(L"Element", 7u, iter);
                    case section_type::code_sec: return ::fast_io::freestanding::my_copy_n(L"Code", 4u, iter);
                    case section_type::data_sec: return ::fast_io::freestanding::my_copy_n(L"Data", 4u, iter);
                    case section_type::data_count_sec: return ::fast_io::freestanding::my_copy_n(L"Data Count", 10u, iter);
                    case section_type::tag_sec: return ::fast_io::freestanding::my_copy_n(L"Tag", 3u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char8_t>)
            {
                switch(sectype)
                {
                    case section_type::custom_sec: return ::fast_io::freestanding::my_copy_n(u8"Custom", 6u, iter);
                    case section_type::type_sec: return ::fast_io::freestanding::my_copy_n(u8"Type", 4u, iter);
                    case section_type::import_sec: return ::fast_io::freestanding::my_copy_n(u8"Import", 6u, iter);
                    case section_type::function_sec: return ::fast_io::freestanding::my_copy_n(u8"Function", 8u, iter);
                    case section_type::table_sec: return ::fast_io::freestanding::my_copy_n(u8"Table", 5u, iter);
                    case section_type::memory_sec: return ::fast_io::freestanding::my_copy_n(u8"Memory", 6u, iter);
                    case section_type::global_sec: return ::fast_io::freestanding::my_copy_n(u8"Global", 6u, iter);
                    case section_type::export_sec: return ::fast_io::freestanding::my_copy_n(u8"Export", 6u, iter);
                    case section_type::start_sec: return ::fast_io::freestanding::my_copy_n(u8"Start", 5u, iter);
                    case section_type::element_sec: return ::fast_io::freestanding::my_copy_n(u8"Element", 7u, iter);
                    case section_type::code_sec: return ::fast_io::freestanding::my_copy_n(u8"Code", 4u, iter);
                    case section_type::data_sec: return ::fast_io::freestanding::my_copy_n(u8"Data", 4u, iter);
                    case section_type::data_count_sec: return ::fast_io::freestanding::my_copy_n(u8"Data Count", 10u, iter);
                    case section_type::tag_sec: return ::fast_io::freestanding::my_copy_n(u8"Tag", 3u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char16_t>)
            {
                switch(sectype)
                {
                    case section_type::custom_sec: return ::fast_io::freestanding::my_copy_n(u"Custom", 6u, iter);
                    case section_type::type_sec: return ::fast_io::freestanding::my_copy_n(u"Type", 4u, iter);
                    case section_type::import_sec: return ::fast_io::freestanding::my_copy_n(u"Import", 6u, iter);
                    case section_type::function_sec: return ::fast_io::freestanding::my_copy_n(u"Function", 8u, iter);
                    case section_type::table_sec: return ::fast_io::freestanding::my_copy_n(u"Table", 5u, iter);
                    case section_type::memory_sec: return ::fast_io::freestanding::my_copy_n(u"Memory", 6u, iter);
                    case section_type::global_sec: return ::fast_io::freestanding::my_copy_n(u"Global", 6u, iter);
                    case section_type::export_sec: return ::fast_io::freestanding::my_copy_n(u"Export", 6u, iter);
                    case section_type::start_sec: return ::fast_io::freestanding::my_copy_n(u"Start", 5u, iter);
                    case section_type::element_sec: return ::fast_io::freestanding::my_copy_n(u"Element", 7u, iter);
                    case section_type::code_sec: return ::fast_io::freestanding::my_copy_n(u"Code", 4u, iter);
                    case section_type::data_sec: return ::fast_io::freestanding::my_copy_n(u"Data", 4u, iter);
                    case section_type::data_count_sec: return ::fast_io::freestanding::my_copy_n(u"Data Count", 10u, iter);
                    case section_type::tag_sec: return ::fast_io::freestanding::my_copy_n(u"Tag", 3u, iter);
                    default: return iter;
                }
            }
            else if constexpr(::std::same_as<char_type, char32_t>)
            {
                switch(sectype)
                {
                    case section_type::custom_sec: return ::fast_io::freestanding::my_copy_n(U"Custom", 6u, iter);
                    case section_type::type_sec: return ::fast_io::freestanding::my_copy_n(U"Type", 4u, iter);
                    case section_type::import_sec: return ::fast_io::freestanding::my_copy_n(U"Import", 6u, iter);
                    case section_type::function_sec: return ::fast_io::freestanding::my_copy_n(U"Function", 8u, iter);
                    case section_type::table_sec: return ::fast_io::freestanding::my_copy_n(U"Table", 5u, iter);
                    case section_type::memory_sec: return ::fast_io::freestanding::my_copy_n(U"Memory", 6u, iter);
                    case section_type::global_sec: return ::fast_io::freestanding::my_copy_n(U"Global", 6u, iter);
                    case section_type::export_sec: return ::fast_io::freestanding::my_copy_n(U"Export", 6u, iter);
                    case section_type::start_sec: return ::fast_io::freestanding::my_copy_n(U"Start", 5u, iter);
                    case section_type::element_sec: return ::fast_io::freestanding::my_copy_n(U"Element", 7u, iter);
                    case section_type::code_sec: return ::fast_io::freestanding::my_copy_n(U"Code", 4u, iter);
                    case section_type::data_sec: return ::fast_io::freestanding::my_copy_n(U"Data", 4u, iter);
                    case section_type::data_count_sec: return ::fast_io::freestanding::my_copy_n(U"Data Count", 10u, iter);
                    case section_type::tag_sec: return ::fast_io::freestanding::my_copy_n(U"Tag", 3u, iter);
                    default: return iter;
                }
            }
        }
    }  // namespace details

    template <::std::integral char_type>
    inline constexpr char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, section_type>, char_type* iter, section_type sectype) noexcept
    {
        return details::print_section_type_type_impl(iter, sectype);
    }

}  // namespace uwvm::wasm
