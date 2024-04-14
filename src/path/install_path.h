#pragma once
#include <fast_io.h>
#include <fast_io_device.h>
#include <io_device.h>

#include <install-paths/impl.h>

namespace uwvm::path
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    inline void
        init_install_path(char const* argv0) noexcept
    {
        ::uwvm::path::get_module_install_path();

        if(::uwvm::path::module_install_path_df == ::fast_io::dir_file{})
        {
            ::uwvm::path::get_module_install_path_from_argv0(argv0);
            if(::uwvm::path::module_install_path_df == ::fast_io::dir_file{})
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
                                    u8"Cannot get module path.\n"
                                    u8"\033[0m"
                                    u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }
            ::fast_io::io::perr(::uwvm::u8err,
                                u8"\033[0m"
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"uwvm: "
                                u8"\033[91m"
                                u8"[warning] "
#ifdef __MSDOS__
                                u8"\033[37m"
#else
                                u8"\033[97m"
#endif
                                u8"Attempting to obtain installation path through argv0, argv0 parameter may be modified by the user, resulting in inaccuracy."
                                u8"\033[0m"
                                u8"\n");
        }
    }

}  // namespace uwvm::path
