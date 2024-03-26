#pragma once
#if(defined(_WIN32) || defined(__CYGWIN__)) && !defined(_WIN32_WINDOWS)
    #include "win32.h"
#endif

namespace uwvm::consolecp
{
#if(defined(_WIN32) || defined(__CYGWIN__)) && !defined(_WIN32_WINDOWS)
    using set_native_console_io_cp_to_utf8 = set_win32_console_io_cp_to_utf8;
    using enable_native_ansi = enable_win32_ansi;
#else
    struct dummy_set_console_io_cp_to_utf8
    {
    };

    struct dummy_enable_ansi
    {
    };

    using set_native_console_io_cp_to_utf8 = dummy_set_console_io_cp_to_utf8;
    using enable_native_ansi = dummy_enable_ansi;
#endif
}  // namespace uwvm::consolecp
