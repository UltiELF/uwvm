#pragma once
#include <cstddef>
#include <limits>
#include <fast_io.h>
#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include <io_device.h>

#include "../../check_index.h"
#include "../../module.h"
#include "../../../run/features.h"

namespace uwvm::wasm
{
    inline void scan_type_section(::uwvm::wasm::wasm_module& wasmmod, ::std::byte const* begin, ::std::byte const* end) noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer scan_type_section_timer{u8"uwvm: [timer] scan type section"};
#endif
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
        // always check
        if(wasmmod.typesec.sec_begin) [[unlikely]]
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(begin - wasmmod.module_begin),
                                u8") "
                                u8"Duplicate WASM Section: Type."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        wasmmod.typesec.sec_begin = begin;
        wasmmod.typesec.sec_end = end;

        // curr
        auto curr{begin};

        // get type size
        ::std::size_t type_count{};
        auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid type length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }
        }

        // check 64-bit indexes
        ::uwvm::wasm::check_index(type_count);

        wasmmod.typesec.type_count = type_count;
        wasmmod.typesec.types.clear();
        wasmmod.typesec.types.reserve(type_count);

        // jump to functype(0x60)
        curr = reinterpret_cast<::std::byte const*>(next);

        ::std::size_t type_counter{};
        ::uwvm::wasm::function_type ft{};
        for(; curr < end;)
        {
            ::uwvm::wasm::value_type vt{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(vt), curr, sizeof(::uwvm::wasm::value_type));
            switch(vt)
            {
                case ::uwvm::wasm::value_type::functype:
                {  // check func counter
                    if(++type_counter > type_count) [[unlikely]]
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"The number of types resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    // jump to para leb128
                    ++curr;

                    // get para len
                    ::std::size_t para_len{};
                    auto const [next_para, err_para]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                        ::fast_io::mnp::leb128_get(para_len))};
                    switch(err_para)
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid type length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    // check 64-bit indexes
                    ::uwvm::wasm::check_index(para_len);

                    // jump to para1
                    curr = reinterpret_cast<::std::byte const*>(next_para);
                    if(static_cast<::std::size_t>(end - curr) < para_len + 1u || curr > end) [[unlikely]]
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid parameter length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    // set 1st para
                    ft.parameter_begin = reinterpret_cast<value_type_const_may_alias_ptr>(curr);
                    curr += para_len;
                    ft.parameter_end = reinterpret_cast<value_type_const_may_alias_ptr>(curr);
                    for(auto para_curr{ft.parameter_begin}; para_curr != ft.parameter_end; ++para_curr)
                    {
                        switch(*para_curr)
                        {
                            case ::uwvm::wasm::value_type::i32: [[fallthrough]];
                            case ::uwvm::wasm::value_type::i64: [[fallthrough]];
                            case ::uwvm::wasm::value_type::f32: [[fallthrough]];
                            case ::uwvm::wasm::value_type::f64: break;
                            case ::uwvm::wasm::value_type::v128:
                            {
                                if(!::uwvm::features::enable_fixed_width_simd) [[unlikely]]
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
                                        u8"(offset=",
                                        ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                        u8") "
                                        u8"Enter parameter --enable-fixed-width-simd to enable wasm fixed-width simd."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                                break;
                            }
                            case ::uwvm::wasm::value_type::externref: [[fallthrough]];
                            case ::uwvm::wasm::value_type::funcref:
                            {
                                if(!::uwvm::features::enable_reference_types) [[unlikely]]
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
                                        u8"(offset=",
                                        ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                        u8") "
                                        u8"Enter parameter --enable-reference-types to enable wasm reference types."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                                break;
                            }
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
                                        u8"(offset=",
                                        ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                        u8") "
                                        u8"Invalid Type: ",
                                        ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(*para_curr)),
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                        }
                    }

                    // get res len
                    ::std::size_t res_len{};
                    auto const [next_res, err_res]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                      reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                      ::fast_io::mnp::leb128_get(res_len))};
                    switch(err_res)
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid type length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    if(!::uwvm::features::enable_multi_value && res_len > 1) [[unlikely]]
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Enter parameter --enable-multivalue to enable multi-value."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    // check 64-bit indexes
                    ::uwvm::wasm::check_index(res_len);

                    // jump to res1
                    curr = reinterpret_cast<::std::byte const*>(next_res);
                    if(static_cast<::std::size_t>(end - curr) < res_len || curr > end) [[unlikely]]
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid result length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
                    // set 1st para
                    ft.result_begin = reinterpret_cast<value_type_const_may_alias_ptr>(curr);
                    curr += res_len;
                    ft.result_end = reinterpret_cast<value_type_const_may_alias_ptr>(curr);
                    for(auto res_curr{ft.result_begin}; res_curr != ft.result_end; ++res_curr)
                    {
                        switch(*res_curr)
                        {
                            case ::uwvm::wasm::value_type::i32: [[fallthrough]];
                            case ::uwvm::wasm::value_type::i64: [[fallthrough]];
                            case ::uwvm::wasm::value_type::f32: [[fallthrough]];
                            case ::uwvm::wasm::value_type::f64: break;
                            case ::uwvm::wasm::value_type::v128:
                            {
                                if(!::uwvm::features::enable_fixed_width_simd) [[unlikely]]
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
                                        u8"(offset=",
                                        ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                        u8") "
                                        u8"Enter parameter --enable-fixed-width-simd to enable wasm fixed-width simd."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                                break;
                            }
                            case ::uwvm::wasm::value_type::externref: [[fallthrough]];
                            case ::uwvm::wasm::value_type::funcref:
                            {
                                if(!::uwvm::features::enable_reference_types) [[unlikely]]
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
                                        u8"(offset=",
                                        ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                        u8") "
                                        u8"Enter parameter --enable-reference-types to enable wasm reference types."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                                break;
                            }
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
                                        u8"(offset=",
                                        ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                        u8") "
                                        u8"Invalid Type: ",
                                        ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(*res_curr)),
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                        }
                    }

                    wasmmod.typesec.types.push_back_unchecked(ft);
                    break;
                }
#if 0 // future🦄
                case ::uwvm::wasm::value_type::resulttype:
                {
                    ::fast_io::fast_terminate();  // todo
                    break;
                }
#endif
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid Type: ",
                                ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(vt)),
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }
        }

        // check func counter
        if(type_counter != type_count) [[unlikely]]
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"The number of types resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
    }
}  // namespace uwvm
