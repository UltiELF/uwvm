#pragma once
#include <cstddef>
#include <fast_io.h>
#include <io_device.h>
#include "wasm_file.h"
#include "../clpara/parsing_result.h"

#include "detect.h"

namespace uwvm
{
    inline void run() noexcept
    {
        if(!::uwvm::wasm_file_name.size()) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
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
    #ifdef __MSDOS__
                                u8"\033[37m"
    #else
                                u8"\033[97m"
    #endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
    #ifdef __MSDOS__
                                u8"\033[37m"
    #else
                                u8"\033[97m"
    #endif
                                  ,
                                e,
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
#endif
        auto curr{reinterpret_cast<::std::byte*>(::uwvm::wasm_file_loader.begin())};
        auto end{reinterpret_cast<::std::byte const*>(::uwvm::wasm_file_loader.cend())};

        if(static_cast<::std::size_t>(end - curr) < 28U) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"Illegal WASM file format.\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        if(!::uwvm::is_wasm_file_unchecked(curr)) [[unlikely]]
        {
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[31m"
                                u8"[fatal] "
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"Illegal WASM file format.\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        curr += 4U;
        ::uwvm::wasm_version = ::uwvm::detect_wasm_version_unchecked(curr);

    }
}  // namespace uwvm
