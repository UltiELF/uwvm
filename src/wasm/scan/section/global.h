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
    inline void scan_global_section(::uwvm::wasm::wasm_module& wasmmod, ::std::byte const* begin, ::std::byte const* end) noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer scan_global_section_timer{u8"uwvm: [timer] scan global section"};
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

        // check is exist
        if(wasmmod.globalsec.sec_begin) [[unlikely]]
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
                                u8"Duplicate WASM Section: Global."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        wasmmod.globalsec.sec_begin = begin;
        wasmmod.globalsec.sec_end = end;

        // curr
        auto curr{begin};

        // get global size
        ::std::size_t global_count{};
        auto [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                  reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                  ::fast_io::mnp::leb128_get(global_count))};
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
                                u8"Invalid global length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }
        }

        // check 64-bit indexes
        ::uwvm::wasm::check_index(global_count);

        wasmmod.globalsec.global_count = global_count;
        wasmmod.globalsec.types.reserve(global_count);

        // jump to global type
        curr = reinterpret_cast<::std::byte const*>(next);

        ::std::size_t global_counter{};
        for(; curr < end;)
        {
            ::uwvm::wasm::local_global_type lgt{};  // union

            if(++global_counter > global_count) [[unlikely]]
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
                                u8"The number of globals resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            ::uwvm::wasm::value_type vt{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(vt), curr, sizeof(::uwvm::wasm::value_type));

            switch(vt)
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
                                        u8"Enter parameter --enable-fixed-width-simd to enable wasm fixed-width simd."
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
                                        u8"Invalid Value Type: ",
                                        ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(vt)),
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }

            lgt.type.type = vt;

            ++curr;

            ::std::uint_fast8_t ma{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(ma), curr, sizeof(::std::uint_fast8_t));

            if(ma == 1)
            {
                if(!::uwvm::features::enable_mutable_globals) [[unlikely]]
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
                                u8"Enter parameter --enable-mutable-globals to enable import or export -mutable globals."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }
            }
            else if(ma != 0) [[unlikely]]
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
                                u8"Invalid varuint1."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            lgt.type.is_mutable = static_cast<bool>(ma);

            ++curr;

            ::uwvm::wasm::op_basic_type opb{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(opb), curr, sizeof(::uwvm::wasm::op_basic_type));

            lgt.initializer.type_opcode.op = static_cast<::uwvm::wasm::op_basic>(opb);

            ++curr;

            switch(static_cast<::uwvm::wasm::op_basic>(opb))
            {
                // to do
                case ::uwvm::wasm::op_basic::global_get: break;

                case ::uwvm::wasm::op_basic::i32_const: break;
                case ::uwvm::wasm::op_basic::i64_const: break;
                case ::uwvm::wasm::op_basic::f32_const: break;
                case ::uwvm::wasm::op_basic::f64_const: break;

                case ::uwvm::wasm::op_basic::ref_null: break;
                case ::uwvm::wasm::op_basic::ref_func: break;

                case ::uwvm::wasm::op_basic::simd_prefix:
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
                                        u8"Enter parameter --enable-fixed-width-simd to enable wasm fixed-width simd."
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    ::uwvm::wasm::op_exten_type ope{};
                    auto [next_ope, err_ope]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                      reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                      ::fast_io::mnp::leb128_get(ope))};
                    switch(err_ope)
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
                                u8"Invalid extension opcode."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    switch(static_cast<::uwvm::wasm::op_exten_simd>(ope))
                    {
                        case ::uwvm::wasm::op_exten_simd::v128_const: break;
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
                                u8"Invalid extension opcode."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    lgt.initializer.type_opcode.extension = ope;

                    curr = reinterpret_cast<::std::byte const*>(next_ope);

                    if(end - curr < 16) [[unlikely]]
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
                                u8"Invalid v128."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    ::uwvm::wasm::wasm_v128 v128val{};

                    if constexpr(::std::endian::little == ::std::endian::native) 
                    {
                        ::fast_io::freestanding::my_memcpy(__builtin_addressof(v128val), curr, sizeof(::uwvm::wasm::wasm_v128));
                    }
                    else 
                    {
#ifdef __SIZEOF_INT128__
                        __uint128_t v128temp{};
                        ::fast_io::freestanding::my_memcpy(__builtin_addressof(v128temp), curr, sizeof(__uint128_t));
                        v128temp = ::fast_io::little_endian(v128temp);
                        v128val = ::std::bit_cast<decltype(v128val)>(v128temp);
#else
                        ::std::uint_least64_t v128le[2]{};  // low, high
                        ::fast_io::freestanding::my_memcpy(v128le, curr, sizeof(v128le));
                        ::std::uint_least64_t v128be[2]{::fast_io::byte_swap(v128le[1]), ::fast_io::byte_swap(v128le[0])};
                        v128val = ::std::bit_cast<decltype(v128val)>(v128be);
#endif
                    }

                    curr += 16;
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
                                u8"Invalid initializer opcode."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }
            wasmmod.globalsec.types.emplace_back_unchecked(lgt);
        }

        if(global_counter != global_count) [[unlikely]]
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
                                u8"The number of globals resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
    }

}  // namespace uwvm::wasm

