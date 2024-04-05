#pragma once

#include <mach-o/dyld.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdbool.h>

#include <fast_io.h>
#include <fast_io_device.h>

#ifndef PATH_MAX
    #define PATH_MAX 4096
#endif

namespace uwvm::path
{
    /*
     * unknown-apple-darwin
     * https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/dyld.3.html
     * for Mac OS, IOS, watch os, ...
     */
    inline void get_module_install_path() noexcept
    {
        char buffer[PATH_MAX + 1];
        ::std::uint_least32_t size{PATH_MAX};
        if(::fast_io::noexcept_call(::_NSGetExecutablePath, buffer, &size) == -1) { return; }
        char buffer2[PATH_MAX + 1];
        char* resolved{::fast_io::noexcept_call(::realpath, buffer, buffer2)};
        if(!resolved) [[unlikely]] { return; }
        ::uwvm::path::module_path = ::fast_io::u8concat(::fast_io::mnp::code_cvt_os_c_str(resolved));
        auto const begin{strlike_begin(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
        auto curr{strlike_curr(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
        for(; curr != begin; curr--)
        {
            if(auto const c{*curr}; c == u8'/')
            {
                curr++;
                break;
            }
        }
        ::uwvm::path::module_install_path = ::fast_io::u8string{begin, curr};
        ::uwvm::path::module_install_path_df = ::fast_io::dir_file{::uwvm::path::module_install_path};
    }
}  // namespace uwvm::path
