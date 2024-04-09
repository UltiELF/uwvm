#pragma once
#include <cstddef>
#include <fast_io.h>
#include "../wasm/abi.h"

namespace uwvm
{
    inline ::std::size_t wasm_file_ppos{};
    inline ::fast_io::basic_os_c_str_with_known_size<char> wasm_file_name{};

    inline ::fast_io::native_file_loader wasm_file_loader{};
    inline ::uwvm::wasm::abi wasm_abi{};
    inline ::std::uint_least32_t wasm_version{};
}  // namespace uwvm
