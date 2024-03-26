#pragma once
#include <fast_io.h>
#include <fast_io_device.h>

namespace uwvm::path
{
    /*
     * for Windows NT
     */
    inline void get_module_install_path() noexcept
    {
        auto c_peb{::fast_io::win32::nt::nt_get_current_peb()};
        auto& NtImagePath{c_peb->ProcessParameters->ImagePathName};
        if(!NtImagePath.Buffer) [[unlikely]] { return; }
        ::uwvm::path::module_path = ::fast_io::u8concat(
            ::fast_io::mnp::code_cvt(::fast_io::mnp::os_c_str_with_known_size(NtImagePath.Buffer, NtImagePath.Length / sizeof(char16_t))));
        const auto begin{strlike_begin(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
        auto curr{strlike_curr(::fast_io::io_strlike_type<char8_t, ::fast_io::u8string>, ::uwvm::path::module_path)};
        for(; curr != begin; curr--)  // calculate nt, dos or (nt) device path
        {
            if(const auto c{*curr}; c == u8'\\' || c == u8':'/* || c == u8'/'*/) 
            {
                curr++;
                break;
            }
        }
        ::uwvm::path::module_install_path = ::fast_io::u8string{begin, curr};
        ::uwvm::path::module_install_path_df = ::fast_io::dir_file{::uwvm::path::module_install_path};
    }
}  // namespace uwvm::path
