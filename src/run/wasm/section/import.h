#pragma once
#include <cstddef>
#include <limits>
#include <fast_io.h>
#include <io_device.h>
#include "../../check_index.h"
#include "../../../wasm/section/import.h"
#include "../../../clpara/parameters/enable-memory64.h"
#include "type.h"

namespace uwvm
{
    inline ::uwvm::wasm::section::import_section global_import_section{};

    inline void detect_import_section(::std::byte const* begin, ::std::byte const* end) noexcept
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

        if(!global_type_section.sec_begin) [[unlikely]]
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
                                u8"The type section must appear before import section."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        // check is exist
        if(global_import_section.sec_begin) [[unlikely]]
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
                                u8"Duplicate WASM Section: Import."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        global_import_section.sec_begin = begin;
        global_import_section.sec_end = end;

        // curr
        auto curr{begin};

        // get import size
        ::std::size_t import_count{};
        auto [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                  reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                  ::fast_io::mnp::leb128_get(import_count))};
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
        ::uwvm::check_index(import_count);

        global_import_section.import_count = import_count;
        global_import_section.types.reserve(import_count);

        // jump to 1st import leb128
        curr = reinterpret_cast<::std::byte const*>(next);

        ::std::size_t import_counter{};
        ::uwvm::wasm::section::import_type it{};

        ::std::size_t import_func_counter{};
        ::std::size_t import_table_counter{};
        ::std::size_t import_memory_counter{};
        ::std::size_t import_global_counter{};

        for(auto const type_count{::uwvm::global_type_section.type_count}; curr < end;)
        {
            // check import counter
            if(++import_counter > import_count) [[unlikely]]
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
                                u8"The number of types resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            // get module len
            ::std::size_t module_len{};
            auto [next_module, err_module]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(module_len))};
            switch(err_module)
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
            ::uwvm::check_index(module_len);

            // set module
            it.module_begin = next_module;
            it.module_end = next_module + module_len;

            // set curr
            curr = reinterpret_cast<::std::byte const*>(next_module + module_len);

            // check curr
            if(end - curr < static_cast<::std::ptrdiff_t>(3)) [[unlikely]]
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
                                u8"Invalid import module length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            // get name len
            ::std::size_t name_len{};
            auto [next_name, err_name]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                ::fast_io::mnp::leb128_get(name_len))};
            switch(err_name)
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
            ::uwvm::check_index(name_len);

            // set name
            it.name_begin = next_name;
            it.name_end = next_name + name_len;

            // set curr
            curr = reinterpret_cast<::std::byte const*>(next_name + name_len);

            // check curr
            if(end - curr < static_cast<::std::ptrdiff_t>(2)) [[unlikely]]
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
                                u8"Invalid import name length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            ::uwvm::wasm::section::import_basic_type ibt{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(ibt), curr, sizeof(::uwvm::wasm::section::import_basic_type));
            if(!::uwvm::wasm::section::is_valid_import_basic_type(ibt)) [[unlikely]]
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
                                ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(ibt)),
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            it.type = ibt;

            switch(ibt)
            {
                case uwvm::wasm::section::import_basic_type::func:
                {
                    ++import_func_counter;

                    // set curr to leb128
                    ++curr;

                    // get type index
                    ::std::size_t type_index{};
                    auto [next_ti, err_ti]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                    reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                    ::fast_io::mnp::leb128_get(type_index))};
                    switch(err_ti)
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

                    // check
                    if(type_index >= type_count) [[unlikely]]
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

                    it.func_type = __builtin_addressof(global_type_section.types.index_unchecked(type_index));

                    global_import_section.types.push_back_unchecked(it);

                    // set curr
                    curr = reinterpret_cast<::std::byte const*>(next_ti);

                    break;
                }
                case uwvm::wasm::section::import_basic_type::table:
                {
                    ++import_table_counter;

                    ::fast_io::fast_terminate();  // todo
                    break;
                }
                case uwvm::wasm::section::import_basic_type::memory:
                {
                    ++import_memory_counter;

                    ::fast_io::fast_terminate();  // todo
                    break;
                }
                case uwvm::wasm::section::import_basic_type::global:
                {
                    ++import_global_counter;

                    ::fast_io::fast_terminate();  // todo
                    break;
                }
                default: ::fast_io::unreachable();
            }
        }

        // check import counter
        if(import_counter != import_count) [[unlikely]]
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
                                u8"The number of types resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        // reserve
        global_import_section.func_types.reserve(import_func_counter);
        global_import_section.table_types.reserve(import_table_counter);
        global_import_section.memory_types.reserve(import_memory_counter);
        global_import_section.global_types.reserve(import_global_counter);

        for(auto const& i: global_import_section.types)
        {
            switch(i.type)
            {
                case uwvm::wasm::section::import_basic_type::func:
                {
                    global_import_section.func_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case uwvm::wasm::section::import_basic_type::table:
                {
                    global_import_section.table_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case uwvm::wasm::section::import_basic_type::memory:
                {
                    global_import_section.memory_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case uwvm::wasm::section::import_basic_type::global:
                {
                    global_import_section.global_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                default: ::fast_io::unreachable();
            }
        }
    }
}  // namespace uwvm
