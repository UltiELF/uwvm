#pragma once

#if defined(__linux__)
    #include <linux/limits.h>
#else
    #include <limits.h>
#endif

#include <fast_io.h>
#include <fast_io_device.h>

namespace uwvm::path
{
    /*
     * for linux, cygwin and solaris
     */
    inline void get_module_install_path() noexcept
    {
        char buffer[PATH_MAX + 1];
#if defined(__linux__) && defined(__NR_readlink)
        auto resolved{::fast_io::system_call<__NR_readlink, int>("/proc/self/exe", buffer, PATH_MAX)};
        if(resolved == -1) [[unlikely]] { return; }
        buffer[resolved] = '\0';
        ::uwvm::path::module_path = ::fast_io::u8concat(::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str_with_known_size(buffer, resolved)));
#else
        char* resolved{::fast_io::noexcept_call(::realpath,
    #if defined(__sun)
                                                "/proc/self/path/a.out"
    #else
                                                "/proc/self/exe"
    #endif
                                                ,
                                                buffer)};
        if(!resolved) [[unlikely]] { return; }
        ::uwvm::path::module_path = ::fast_io::u8concat(::fast_io::mnp::code_cvt_os_c_str(resolved));
#endif
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
