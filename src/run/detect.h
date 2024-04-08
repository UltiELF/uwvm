#pragma once
#include <cstddef>
#include <fast_io.h>
#include <io_device.h>
#include "wasm_file.h"

namespace uwvm
{
    inline constexpr bool is_wasm_file_unchecked(::std::byte const* curr) noexcept { return ::fast_io::freestanding::my_memcmp(curr, u8"\0asm", 4u) == 0; }

    inline constexpr ::std::uint_least32_t detect_wasm_version_unchecked(::std::byte const* curr) noexcept
    {
        ::std::uint_least32_t temp{};
        ::fast_io::freestanding::my_memcpy(__builtin_addressof(temp), curr, sizeof(::std::uint_least32_t));
        return ::fast_io::little_endian(temp);
    }

    inline constexpr void scan_wasm_file(::std::byte const* begin, ::std::byte const* end) noexcept
    {
        // alias
        using char8_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = char8_t*;
        using char8_t_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = char8_t const*;
        // curr
        auto curr{begin};
        // min size of wasm file format = 4 + 4 + 1 + 1

        if(static_cast<::std::size_t>(end - curr) < 10U) [[unlikely]]
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

        // check wasm magic number

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

        // get wasm version

        curr += 4U;
        ::uwvm::wasm_version = ::uwvm::detect_wasm_version_unchecked(curr);

        // get first sectino
        ++curr;
    }
}  // namespace uwvm
