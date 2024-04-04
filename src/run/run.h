#pragma once
#include <cstddef>
#include <fast_io.h>
#include <io_device.h>
#include "wasm_file.h"
#include "../clpara/parsing_result.h"

namespace uwvm
{
    inline void run() noexcept
    { 
        ::uwvm::wasm_file_loader = ::fast_io::native_file_loader{::uwvm::wasm_file_name};
    }
}  // namespace uwvm
