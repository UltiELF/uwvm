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
        if(!::uwvm::wasm_file_name.size())
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
                                u8"\033[97m"
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
                                u8"\033[97m"
                                u8"No input file.\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
#ifdef __cpp_exceptions
        try
        {
#endif
            ::uwvm::wasm_file_loader = ::fast_io::native_file_loader{::uwvm::wasm_file_name};
#ifdef __cpp_exceptions
        }
        catch(::fast_io::error e)
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
                                u8"\033[97m"
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
                                u8"\033[97m",
                                e,
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
#endif
    }
}  // namespace uwvm
