#pragma once
#include <cstddef>
#include <limits>
#include <fast_io.h>
#include <io_device.h>
#include "../../../wasm/section/type.h"
#include "../../../clpara/parameters/enable-memory64.h"

namespace uwvm
{
    inline ::uwvm::wasm::section::type_section global_type_section{};

    inline void detect_type_section(::std::byte const* begin, ::std::byte const* end) noexcept
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
        using value_type_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::wasm::value_type*;
        using value_type_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::wasm::value_type const*;

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

        // curr
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
        global_type_section.type_count = type_count;
        global_type_section.types.reserve(type_count);

        // set curr to next
        curr = reinterpret_cast<::std::byte const*>(next);
        if(curr < end) [[likely]]
        {
            ::uwvm::wasm::value_type vt1st{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(vt1st), curr, sizeof(::uwvm::wasm::value_type));
            if(vt1st != ::uwvm::wasm::value_type::functype) [[unlikely]]
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
                                u8"The first type must be function: ",
                                ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(vt1st)),
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }
        }

#if 0
        constexpr auto simd_n{::fast_io::intrinsics::optimal_simd_vector_run_with_cpu_instruction_size_with_mask_countr};
        if constexpr(simd_n == 64U) {}
        else if constexpr(simd_n == 32U) {}
        else if constexpr(simd_n == 16U) {}
        else
#endif
        {  // count 0x60
            ::std::size_t func_count{};

            // jump to curr + 1
            ++curr;
            func_count = 1U;
            ::uwvm::wasm::section::function_type tmp_ft{.parameter_begin{reinterpret_cast<value_type_const_may_alias_ptr>(curr)}};
            for(; curr < end; ++curr)
            {
                ::uwvm::wasm::value_type vt{};
                ::fast_io::freestanding::my_memcpy(__builtin_addressof(vt), curr, sizeof(::uwvm::wasm::value_type));
                if(vt == ::uwvm::wasm::value_type::functype)
                {
                    if(++func_count > type_count) [[unlikely]]
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
                                u8"The number of types exceeds the limit: ",
                                type_count,
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    // end
                    tmp_ft.parameter_end = reinterpret_cast<value_type_const_may_alias_ptr>(curr - 2);

                    // check
                    if(tmp_ft.parameter_begin > tmp_ft.parameter_end) [[unlikely]]
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
                                u8"Invalid function type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    // result
                    ::uwvm::wasm::value_type rt{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(rt), curr - 1, sizeof(::uwvm::wasm::value_type));
                    tmp_ft.result_type = rt;

                    global_type_section.types.push_back_unchecked(tmp_ft);

                    tmp_ft.parameter_begin = reinterpret_cast<value_type_const_may_alias_ptr>(curr + 1);
                }
                else
                {
                    if(!::uwvm::wasm::is_valid_value_type(vt)) [[unlikely]]
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
                                u8"Invalid Type: ",
                                ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(vt)),
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
                }
            }

            // push back last type
            tmp_ft.parameter_end = reinterpret_cast<value_type_const_may_alias_ptr>(curr - 2);

            // check
            if(tmp_ft.parameter_begin > tmp_ft.parameter_end) [[unlikely]]
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
                                u8"Invalid function type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            // result
            ::uwvm::wasm::value_type rt{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(rt), curr - 1, sizeof(::uwvm::wasm::value_type));
            tmp_ft.result_type = rt;
            global_type_section.types.push_back_unchecked(tmp_ft);
        }
    }
}  // namespace uwvm
