#pragma once
#include <cstddef>
#include <fast_io.h>
#include <io_device.h>

namespace uwvm
{
    inline ::std::size_t wasm_file_ppos{};
    inline ::fast_io::basic_os_c_str_with_known_size<char> wasm_file_name{};

    inline void run() noexcept 
    { 
        // ::fast_io::perrln(::uwvm::u8err, wasm_file_ppos, u8" ", ::fast_io::mnp::code_cvt(wasm_file_name));
    }
}  // namespace uwvm
