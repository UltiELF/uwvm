#pragma once
#include <cstddef>
#include <fast_io.h>

namespace uwvm
{
    inline ::std::size_t wasm_file_ppos{};
    inline ::fast_io::basic_os_c_str_with_known_size<char> wasm_file_name{};

    inline ::fast_io::native_file_loader wasm_file_loader{};

}  // namespace uwvm
