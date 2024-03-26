#include "impl.h"

#if(defined(_WIN32) || defined(__CYGWIN__)) && !defined(_WIN32_WINDOWS)
static ::uwvm::consolecp::set_native_console_io_cp_to_utf8 set_native_console_io_cp_to_utf8_ele{};
static ::uwvm::consolecp::enable_native_ansi enable_native_ansi_ele{};
#endif
