#pragma once
#include <cstdint>

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
        data_count_sec
    };

    inline constexpr ::std::uint_fast8_t get_section_value(section_type sectype) noexcept { return static_cast<::std::uint_fast8_t>(sectype); }

    inline constexpr bool is_valid_section(section_type sectype) noexcept
    {
        return static_cast<::std::uint_fast8_t>(sectype) <= static_cast<::std::uint_fast8_t>(section_type::data_count_sec);
    }
}  // namespace uwvm::wasm
