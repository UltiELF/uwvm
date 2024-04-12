#pragma once
#include <cstddef>
#include <fast_io.h>
#include <io_device.h>
#include "wasm_file.h"
#include "../wasm/section.h"

#include "check_index.h"
#include "wasm/section.h"

namespace uwvm
{
    inline 
#if defined(__has_builtin)
    #if __has_builtin(__builtin_memcmp)
        constexpr
    #endif
#endif
        bool
        is_wasm_file_unchecked(::std::byte const* curr) noexcept
    {
        return ::fast_io::freestanding::my_memcmp(curr, u8"\0asm", 4u) == 0;
    }

    inline 
#if defined(__has_builtin)
    #if __has_builtin(__builtin_memcpy)
        constexpr
    #endif
#endif
        ::std::uint_least32_t
        detect_wasm_version_unchecked(::std::byte const* curr) noexcept
    {
        ::std::uint_least32_t temp{};
        ::fast_io::freestanding::my_memcpy(__builtin_addressof(temp), curr, sizeof(::std::uint_least32_t));
        return ::fast_io::little_endian(temp);
    }

    inline void scan_wasm_file(::std::byte const* begin, ::std::byte const* end) noexcept
    {
        // alias def
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

        // min size of wasm file format = 4 + 4
        // check wasm magic number
        if(static_cast<::std::size_t>(end - curr) < 8U || !::uwvm::is_wasm_file_unchecked(curr)) [[unlikely]]
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

        // get first section
        curr += 4U;

        if(end - curr < 2) [[unlikely]]
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
                                u8"No WASM sections found.\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        // objdump
        for(;;)
        {
            // get section type
            ::std::uint_fast8_t sec_num{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(sec_num), curr, sizeof(::std::uint_fast8_t));

            // get section length
            ++curr;
            ::std::size_t sec_len{};
            auto [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                      reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                      ::fast_io::mnp::leb128_get(sec_len))};
            switch(err)
            {
                case ::fast_io::parse_code::ok: break;
                default:
                    [[unlikely]]
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
                                u8"Invalid section length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }

            // check 64-bit indexes
            ::uwvm ::check_index(sec_len);

            // set curr to next
            curr = reinterpret_cast<::std::byte const*>(next);

            // check length
            if(end - curr < sec_len) [[unlikely]]
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
                                u8"Invalid section length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            // sec end
            auto const sec_end{curr + sec_len};

            // check
            auto const sec_type{static_cast<::uwvm::wasm::section_type>(sec_num)};
            switch(sec_type)
            {
                case ::uwvm::wasm::section_type::custom_sec: break;
                case ::uwvm::wasm::section_type::type_sec:
                {
                    ::uwvm::detect_type_section(curr, sec_end);
                    break;
                }
                case ::uwvm::wasm::section_type::import_sec:
                {
                    ::uwvm::detect_import_section(curr, sec_end);
                    break;
                }
                case ::uwvm::wasm::section_type::function_sec: break;
                case ::uwvm::wasm::section_type::table_sec: break;
                case ::uwvm::wasm::section_type::memory_sec: break;
                case ::uwvm::wasm::section_type::global_sec: break;
                case ::uwvm::wasm::section_type::export_sec: break;
                case ::uwvm::wasm::section_type::start_sec: break;
                case ::uwvm::wasm::section_type::element_sec: break;
                case ::uwvm::wasm::section_type::code_sec: break;
                case ::uwvm::wasm::section_type::data_sec: break;
                case ::uwvm::wasm::section_type::data_count_sec: break;
                default:
                    [[unlikely]]
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
                                u8"Unknown WASM section: ",
                                ::fast_io::mnp::hex0x<true>(sec_num),
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }

            // set curr
            curr = sec_end;

            // check next section
            if(auto const dif{end - curr}; dif == 0U) { break; }
            else if(dif < 2U) [[unlikely]]
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
                                u8"Unable to read leb128."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }
        }
    }
}  // namespace uwvm
