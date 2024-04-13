#pragma once
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/sysctl.h>
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
     * for DragonFlyBSD ,FreeBSD, NetBSD
     */
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    inline void
        get_module_install_path() noexcept
    {
        char buffer1[PATH_MAX + 1];
        char buffer2[PATH_MAX + 1];
        char* resolved = NULL;
        int length = -1;

#if defined(__NetBSD__)
        int mib[4]{CTL_KERN, KERN_PROC_ARGS, -1, KERN_PROC_PATHNAME};
#else
        int mib[4]{CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
#endif
        ::std::size_t size{PATH_MAX};

        if(::fast_io::noexcept_call(::sysctl, mib, 4, buffer1, &size, nullptr, 0) != 0) { return; }

        resolved = ::fast_io::noexcept_call(::realpath, buffer1, buffer2);

        if(!resolved) { return; }

        ::uwvm::path::module_path = ::fast_io::u8concat_fast_io(::fast_io::mnp::code_cvt_os_c_str(resolved));
        auto const begin{strlike_begin(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
        auto curr{strlike_curr(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
        for(; curr != begin; curr--)  // calculate nt or dos path
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
