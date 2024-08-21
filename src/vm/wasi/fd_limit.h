#pragma once
#include <fast_io.h>
#include <fast_io_device.h>

namespace uwvm::vm::wasi
{
    inline ::std::size_t wasi_fd_limit{1024};
}
