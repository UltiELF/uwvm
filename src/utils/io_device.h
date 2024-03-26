#pragma once

#include <fast_io.h>
#include <fast_io_device.h>

namespace uwvm
{
#ifndef __AVR__
    inline ::fast_io::u8native_io_observer u8in{::fast_io::u8in()};
    inline ::fast_io::u8native_io_observer u8out{::fast_io::u8out()};
    inline ::fast_io::u8native_io_observer u8err{::fast_io::u8err()};
#else
    inline ::fast_io::u8c_io_observer u8in{::fast_io::u8c_stdin()};
    inline ::fast_io::u8c_io_observer u8out{::fast_io::u8c_stdout()};
    inline ::fast_io::u8c_io_observer u8err{::fast_io::u8c_stderr()};
#endif
}  // namespace uwvm
