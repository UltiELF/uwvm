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
    inline void get_module_install_path_from_argv0(char const* argv0) noexcept
    {
#if !defined(_WIN32) && !defined(__MSDOS__) && !defined(__wasm__)
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
            ::realpath(argv0, newpath);
            if(!access(newpath, F_OK))
            {
                newpath[PATH_MAX - 1] = 0;
                ::uwvm::path::module_path = ::fast_io::u8concat(::fast_io::mnp::code_cvt_os_c_str(newpath));
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
        else if(strchr(argv0, path_separator))
        {
            ::getcwd(newpath2, PATH_MAX);
            strncat(newpath2, path_separator_as_string, PATH_MAX + 256);
            strncat(newpath2, argv0, PATH_MAX + 256);
            ::realpath(newpath2, newpath);
            if(!access(newpath, F_OK))
            {
                newpath[PATH_MAX - 1] = 0;
                ::uwvm::path::module_path = ::fast_io::u8concat(::fast_io::mnp::code_cvt_os_c_str(newpath));
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
            char* save_path{::getenv("PATH")};
            for(pathitem = ::strtok_r(save_path, path_list_separator, &saveptr); pathitem; pathitem = ::strtok_r(NULL, path_list_separator, &saveptr))
            {
                strncpy(newpath2, pathitem, PATH_MAX + 256);
                strncat(newpath2, path_separator_as_string, PATH_MAX + 256);
                strncat(newpath2, argv0, PATH_MAX + 256);
                ::realpath(newpath2, newpath);
                if(!access(newpath, F_OK))
                {
                    newpath[PATH_MAX - 1] = 0;
                    ::uwvm::path::module_path = ::fast_io::u8concat(::fast_io::mnp::code_cvt_os_c_str(newpath));
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

#endif
    }
}  // namespace uwvm::path
