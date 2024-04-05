#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <fast_io.h>

namespace uwvm::consolecp
{

    // https://learn.microsoft.com/en-us/windows/win32/intl/code-page-identifiers

    struct set_win32_console_io_cp_to_utf8
    {
        inline static constexpr ::std::uint_least32_t utf8_coding{65001u};  // utf8

        ::std::uint_least32_t output{};
        ::std::uint_least32_t input{};

#if __has_cpp_attribute(__gnu__::__cold__)
        [[__gnu__::__cold__]]
#endif
        set_win32_console_io_cp_to_utf8() noexcept
        {
            output = ::fast_io::win32::GetConsoleOutputCP();
            input = ::fast_io::win32::GetConsoleCP();
            if(output != utf8_coding) { ::fast_io::win32::SetConsoleOutputCP(utf8_coding); }
            if(input != utf8_coding) { ::fast_io::win32::SetConsoleCP(utf8_coding); }
        }

#if __has_cpp_attribute(__gnu__::__cold__)
        [[__gnu__::__cold__]]
#endif
#if __has_cpp_attribute(__gnu__::__pure__)
        [[__gnu__::__pure__]]
#endif
        ~set_win32_console_io_cp_to_utf8()
        {
            if(output != utf8_coding) { ::fast_io::win32::SetConsoleOutputCP(output); }
            if(input != utf8_coding) { ::fast_io::win32::SetConsoleCP(input); }
        }
    };

    struct enable_win32_ansi
    {
        inline static constexpr ::std::uint_least32_t enable_virtual_terminal_processing{0x0004u};

        ::std::uint_least32_t out_omode{};
        ::std::uint_least32_t err_omode{};

        void* out_handle{};
        void* err_handle{};

#if __has_cpp_attribute(__gnu__::__cold__)
        [[__gnu__::__cold__]]
#endif
        enable_win32_ansi() noexcept
        {
            out_handle = ::fast_io::win32::GetStdHandle(::fast_io::win32_stdout_number);
            err_handle = ::fast_io::win32::GetStdHandle(::fast_io::win32_stderr_number);
            ::fast_io::win32::GetConsoleMode(out_handle, &out_omode);
            ::fast_io::win32::GetConsoleMode(err_handle, &err_omode);
            ::fast_io::win32::SetConsoleMode(out_handle, out_omode | enable_virtual_terminal_processing);
            ::fast_io::win32::SetConsoleMode(err_handle, err_omode | enable_virtual_terminal_processing);
        }
    };

}  // namespace uwvm::consolecp
