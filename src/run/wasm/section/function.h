#pragma once
#include <cstddef>
#include <limits>
#include <fast_io.h>
#ifdef UWVM_TIMER
    #include <fast_io_driver/timer.h>
#endif
#include <io_device.h>

#include "../../wasm_file.h"

#include "../../check_index.h"
#include "../../../wasm/section/function.h"
#include "../../../clpara/parameters/enable-memory64.h"

namespace uwvm
{
    inline void scan_function_section(::std::byte const* begin, ::std::byte const* end) noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer scan_function_section_timer{u8"uwvm: [timer] scan function section"};
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

        if(!::uwvm::global_wasm_module.typesec.sec_begin) [[unlikely]]
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
                                u8"The type section must appear before function section."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        // check is exist
        if(::uwvm::global_wasm_module.functionsec.sec_begin) [[unlikely]]
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
                                u8"Duplicate WASM Section: Function."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        ::uwvm::global_wasm_module.functionsec.sec_begin = begin;
        ::uwvm::global_wasm_module.functionsec.sec_end = end;

        // curr
        auto curr{begin};

        // get function size
        ::std::size_t function_count{};
        auto [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                  reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                  ::fast_io::mnp::leb128_get(function_count))};
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
                                u8"Invalid function length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }
        }

        // check 64-bit indexes
        ::uwvm::check_index(function_count);

        ::uwvm::global_wasm_module.functionsec.function_count = function_count;
        ::uwvm::global_wasm_module.functionsec.types.reserve(function_count);

        // jump to func type
        curr = reinterpret_cast<::std::byte const*>(next);

        auto const type_table_basic_index{::uwvm::global_wasm_module.typesec.types.cbegin()};
        auto const type_table_size{::uwvm::global_wasm_module.typesec.types.size()};

        ::std::size_t function_counter{};
        for(; curr < end;)
        {
            if(++function_counter > function_count) [[unlikely]]
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
                                u8"The number of functions resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            ::std::size_t type_index_len{};
            auto [next_type_index, err_type_index]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                            reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                            ::fast_io::mnp::leb128_get(type_index_len))};
            switch(err_type_index)
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
                                u8"Invalid index length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }

            // check
            if(type_index_len >= type_table_size) [[unlikely]]
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
                                u8"Invalid type index."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            ::uwvm::global_wasm_module.functionsec.types.emplace_back_unchecked(type_table_basic_index + type_index_len);
            // jump to para1
            curr = reinterpret_cast<::std::byte const*>(next_type_index);
        }
        // check func counter
        if(function_counter != function_count) [[unlikely]]
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
                                u8"The number of functions resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
    }

}  // namespace uwvm

