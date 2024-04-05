#pragma once
#include <fast_io.h>
#include <fast_io_device.h>
#include <fast_io_dsal/string.h>

namespace uwvm::path
{
    inline char const*
#if __has_cpp_attribute(__gnu__::__const__)
        [[__gnu__::__const__]]
#endif
        argv0{};
    inline ::fast_io::dir_file
#if __has_cpp_attribute(__gnu__::__const__)
        [[__gnu__::__const__]]
#endif
        module_install_path_df{};
    inline ::fast_io::u8string
#if __has_cpp_attribute(__gnu__::__const__)
        [[__gnu__::__const__]]
#endif
        module_path{};
    inline ::fast_io::u8string
#if __has_cpp_attribute(__gnu__::__const__)
        [[__gnu__::__const__]]
#endif
        module_install_path{};
}  // namespace uwvm::path
