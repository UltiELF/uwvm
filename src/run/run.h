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
#if 0
        ::fast_io::perrln(::uwvm::u8err, wasm_file_ppos, u8" ", ::fast_io::mnp::code_cvt(wasm_file_name));
        for(auto i{wasm_file_ppos + 1}; i < ::uwvm::parsing_result.size(); ++i)
        {
            ::fast_io::perrln(::uwvm::u8err, ::fast_io::mnp::code_cvt(::uwvm::parsing_result[i].str));
        }
#endif
    }
}  // namespace uwvm
