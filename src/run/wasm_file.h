#pragma once
#include <cstddef>
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
#include <fast_io_dsal/string.h>
#include "../wasm/abi.h"
#include "../wasm/module.h"
#include "mode.h"

namespace uwvm
{
    inline ::fast_io::native_file_loader wasm_file_loader{};

    inline ::uwvm::mode running_mode{::uwvm::mode::interpreter};

    inline ::uwvm::wasm::abi wasm_abi{::uwvm::wasm::abi::detect};
    inline ::uwvm::wasm::wasm_module global_wasm_module{};

}  // namespace uwvm
