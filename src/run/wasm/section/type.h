#pragma once
#include <cstddef>
#include <limits>
#include <fast_io.h>
#include <io_device.h>
#include "../../../wasm/section/type.h"
#include "../../../clpara/parameters/enable-memory64.h"

namespace uwvm
{
    inline ::uwvm::section::type_section global_type_section{};

	inline 
#if defined(__has_builtin)
    #if __has_builtin(__builtin_memcmp) && __has_builtin(__builtin_memcpy)
        constexpr
    #endif
#endif
        void
        detect_type_section(::std::byte const* begin, ::std::byte const* end) noexcept
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

        // check is exist
        if(global_type_section.sec_begin) [[unlikely]] 
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
                                u8"Duplicate WASM Section: Type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        global_type_section.sec_begin = begin;
        global_type_section.sec_end = end;

        //curr
        auto curr{begin};

        // get type size
        ::std::size_t type_count{};
        auto [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                  reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                  ::fast_io::mnp::leb128_get(type_count))};
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
                                u8"Invalid type length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }
        }

        // check 64-bit indexes
        if constexpr(sizeof(::std::size_t) == 8)
        {
            auto const has_enable_memory64_alias{::uwvm::parameter::details::enable_memory64_is_exist};
            constexpr auto u32max{static_cast<::std::size_t>(::std::numeric_limits<::std::uint_least32_t>::max())};
            if(!has_enable_memory64_alias && type_count > u32max) [[unlikely]]
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
                                u8"64-bit memory addresses requires memory64 feature."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }
        }

        global_type_section.count = type_count;

#ifdef _DEBUG
        ::fast_io::io::perrln(::uwvm::u8err, type_count);
#endif
    }
}