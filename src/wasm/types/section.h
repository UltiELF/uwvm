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
        return static_cast<::std::uint_fast8_t>(sectype) <= static_cast<::std::uint_fast8_t>(section_type::data_count_sec);
    }

    inline constexpr ::fast_io::u8string_view get_section_u8name(section_type sectype) noexcept 
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
            default: return {};
        }
    }
}  // namespace uwvm::wasm
