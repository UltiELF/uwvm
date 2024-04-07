#pragma once
#include <cstdint>
#include <fast_io_core_impl/terminate.h>

namespace uwvm::wasm
{
    enum class section_type : ::std::uint_least8_t
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

    inline constexpr ::std::uint_least8_t get_section_value(section_type sectype) noexcept { return static_cast<::std::uint_least8_t>(sectype); }
}  // namespace uwvm::wasm
