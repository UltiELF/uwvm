#pragma once

#include <limits.h>
#include <stdlib.h>
#include <cstring>

#include <fast_io.h>
#include <fast_io_device.h>

#ifndef PATH_MAX
    #define PATH_MAX 4096
#endif

namespace uwvm::path
{
#if __has_cpp_attribute(__gnu__::__cold__)
    [[__gnu__::__cold__]]
#endif
    inline void
        get_module_install_path_from_argv0(char const* argv0) noexcept
    {
        if(!argv0) [[unlikely]] { return; }

        char newpath[PATH_MAX + 256 + 1];
        char newpath2[PATH_MAX + 256 + 1];

        newpath[PATH_MAX + 256] = '0';
        newpath2[PATH_MAX + 256] = '0';

        constexpr char path_separator = '/';
        char path_separator_as_string[2] = "/";
        char path_list_separator[8] = ":";  // could be ":; "
        if(argv0[0] == path_separator)
        {
            [[maybe_unused]] auto const unused1{::fast_io::noexcept_call(realpath, argv0, newpath)};
            if(!::fast_io::noexcept_call(access, newpath, F_OK))
            {
                newpath[PATH_MAX - 1] = 0;
                ::uwvm::path::module_path = ::fast_io::u8concat_fast_io(::fast_io::mnp::code_cvt_os_c_str(newpath));
                auto const begin2{strlike_begin(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
                auto curr{strlike_curr(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
                for(; curr != begin2; curr--)  // calculate dos path
                {
                    if(auto const c{*curr}; c == u8'/')
                    {
                        curr++;
                        break;
                    }
                }
                ::uwvm::path::module_install_path = ::fast_io::u8string{begin2, curr};
                ::uwvm::path::module_install_path_df = ::fast_io::dir_file{::uwvm::path::module_install_path};

                return;
            }
            else { return; }
        }
        else if(
#if __has_builtin(__builtin_strchr)
            __builtin_strchr(argv0, static_cast<int>(path_separator))
#else
            strchr(argv0, static_cast<int>(path_separator))
#endif
        )
        {
            [[maybe_unused]] auto const unused1{::fast_io::noexcept_call(getcwd, newpath2, PATH_MAX)};
            ::fast_io::noexcept_call(strncat, newpath2, path_separator_as_string, PATH_MAX + 256);
            ::fast_io::noexcept_call(strncat, newpath2, argv0, PATH_MAX + 256);
            [[maybe_unused]] auto const unused2{::fast_io::noexcept_call(realpath, newpath2, newpath)};
            if(!::fast_io::noexcept_call(access, newpath, F_OK))
            {
                newpath[PATH_MAX - 1] = 0;
                ::uwvm::path::module_path = ::fast_io::u8concat_fast_io(::fast_io::mnp::code_cvt_os_c_str(newpath));
                auto const begin2{strlike_begin(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
                auto curr{strlike_curr(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
                for(; curr != begin2; curr--)  // calculate dos path
                {
                    if(auto const c{*curr}; c == u8'/')
                    {
                        curr++;
                        break;
                    }
                }
                ::uwvm::path::module_install_path = ::fast_io::u8string{begin2, curr};
                ::uwvm::path::module_install_path_df = ::fast_io::dir_file{::uwvm::path::module_install_path};

                return;
            }
            else { return; }
        }
        else
        {
            char* saveptr;
            char* pathitem;
            char* save_path{::fast_io::noexcept_call(getenv, "PATH")};
            for(pathitem = ::fast_io::noexcept_call(strtok_r, save_path, path_list_separator, &saveptr); pathitem;
                pathitem = ::fast_io::noexcept_call(strtok_r, nullptr, path_list_separator, &saveptr))
            {
                ::fast_io::noexcept_call(strncpy, newpath2, pathitem, PATH_MAX + 256);
                ::fast_io::noexcept_call(strncat, newpath2, path_separator_as_string, PATH_MAX + 256);
                ::fast_io::noexcept_call(strncat, newpath2, argv0, PATH_MAX + 256);
                [[maybe_unused]] auto const unused1{::realpath(newpath2, newpath)};
                if(!::fast_io::noexcept_call(access, newpath, F_OK))
                {
                    newpath[PATH_MAX - 1] = 0;
                    ::uwvm::path::module_path = ::fast_io::u8concat_fast_io(::fast_io::mnp::code_cvt_os_c_str(newpath));
                    auto const begin2{strlike_begin(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
                    auto curr{strlike_curr(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
                    for(; curr != begin2; curr--)  // calculate dos path
                    {
                        if(auto const c{*curr}; c == u8'/')
                        {
                            curr++;
                            break;
                        }
                    }
                    ::uwvm::path::module_install_path = ::fast_io::u8string{begin2, curr};
                    ::uwvm::path::module_install_path_df = ::fast_io::dir_file{::uwvm::path::module_install_path};
                    return;
                }
            }  // end for
            return;

        }  // end else
    }
}  // namespace uwvm::path
