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
    inline void scan_export_section(::uwvm::wasm::wasm_module& wasmmod, ::std::byte const* begin, ::std::byte const* end) noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer scan_export_section_timer{u8"uwvm: [timer] scan export section"};
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
        if(wasmmod.exportsec.sec_begin) [[unlikely]]
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
                                u8"(",
                                ::fast_io::mnp::addrvw(begin - wasmmod.module_begin),
                                u8") "
                                u8"Duplicate WASM Section: Export."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        wasmmod.exportsec.sec_begin = begin;
        wasmmod.exportsec.sec_end = end;

        // curr
        auto curr{begin};

        // get export size
        ::std::size_t export_count{};
        ::std::size_t export_func_counter{};
        ::std::size_t export_table_counter{};
        ::std::size_t export_memory_counter{};
        ::std::size_t export_global_counter{};
        ::std::size_t export_tag_counter{};

        auto [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                  reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                  ::fast_io::mnp::leb128_get(export_count))};
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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid export length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }
        }

        // check 64-bit indexes
        ::uwvm::wasm::check_index(export_count);

        wasmmod.exportsec.export_count = export_count;
        wasmmod.exportsec.types.clear();
        wasmmod.exportsec.types.reserve(export_count);

        // jump to export type
        curr = reinterpret_cast<::std::byte const*>(next);

        ::std::size_t export_counter{};
        for(::uwvm::wasm::export_type et{}; curr < end;)
        {
            if(++export_counter > export_count) [[unlikely]]
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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"The number of exports resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            ::std::size_t export_name_len{};
            auto [next_en, err_en]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                            reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                            ::fast_io::mnp::leb128_get(export_name_len))};
            switch(err_en)
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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid index length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }

            // check
            ::uwvm::wasm::check_index(export_name_len);

            curr = reinterpret_cast<::std::byte const*>(next_en);

            if(end - curr < export_name_len + 2) [[unlikely]]
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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid index length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            et.name_begin = reinterpret_cast<char8_t_const_may_alias_ptr>(curr);
            curr += export_name_len;
            et.name_end = reinterpret_cast<char8_t_const_may_alias_ptr>(curr);

            ::uwvm::wasm::extern_kind ek{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(ek), curr, sizeof(::uwvm::wasm::extern_kind));

            switch(ek)
            {
                case ::uwvm::wasm::extern_kind::func:
                {
                    ++export_func_counter;
                    break;
                }
                case ::uwvm::wasm::extern_kind::table:
                {
                    ++export_table_counter;
                    break;
                }
                case ::uwvm::wasm::extern_kind::memory:
                {
                    ++export_memory_counter;
                    break;
                }
                case ::uwvm::wasm::extern_kind::global:
                {
                    ++export_global_counter;
                    break;
                }
                case ::uwvm::wasm::extern_kind::tag:
                {
                    ++export_tag_counter;
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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid Type: ",
                                ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(ek)),
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }

            et.kind = ek;
            ++curr;

            ::std::size_t index{};
            auto [next_index, err_index]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                  reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                  ::fast_io::mnp::leb128_get(index))};
            switch(err_index)
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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid index length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }

            et.index = index;
            wasmmod.exportsec.types.push_back_unchecked(et);

            curr = reinterpret_cast<::std::byte const*>(next_index);
        }

        // check export counter
        if(export_counter != export_count) [[unlikely]]
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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"The number of exports resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        // clear
        wasmmod.exportsec.func_types.clear();
        wasmmod.exportsec.table_types.clear();
        wasmmod.exportsec.memory_types.clear();
        wasmmod.exportsec.global_types.clear();
        wasmmod.exportsec.tag_types.clear();

        // reserve
        wasmmod.exportsec.func_types.reserve(export_func_counter);
        wasmmod.exportsec.table_types.reserve(export_table_counter);
        wasmmod.exportsec.memory_types.reserve(export_memory_counter);
        wasmmod.exportsec.global_types.reserve(export_global_counter);
        wasmmod.exportsec.tag_types.reserve(export_tag_counter);

        for(auto const& i: wasmmod.exportsec.types)
        {
            switch(i.kind)
            {
                case ::uwvm::wasm::extern_kind::func:
                {
                    wasmmod.exportsec.func_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case ::uwvm::wasm::extern_kind::table:
                {
                    wasmmod.exportsec.table_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case ::uwvm::wasm::extern_kind::memory:
                {
                    wasmmod.exportsec.memory_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case ::uwvm::wasm::extern_kind::global:
                {
                    wasmmod.exportsec.global_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case ::uwvm::wasm::extern_kind::tag:
                {
                    wasmmod.exportsec.tag_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                default: ::fast_io::unreachable();
            }
        }
    }

}  // namespace uwvm::wasm

