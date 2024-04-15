#pragma once
#include <cstddef>
#include <fast_io.h>
#include <fast_io_dsal/cstring_view.h>
#include "../wasm/abi.h"
#include "../wasm/module.h"
#include "mode.h"

namespace uwvm
{
    inline ::std::size_t wasm_file_ppos{};
    inline ::fast_io::cstring_view wasm_file_name{};

    inline ::fast_io::native_file_loader wasm_file_loader{};

    inline ::uwvm::mode running_mode{::uwvm::mode::objdump};

    inline ::uwvm::wasm::abi wasm_abi{};
    inline ::uwvm::wasm::wasm_module global_wasm_module{};

}  // namespace uwvm
