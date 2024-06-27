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
    inline void scan_data_section(::uwvm::wasm::wasm_module& wasmmod, ::std::byte const* begin, ::std::byte const* end) noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer scan_data_section_timer{u8"uwvm: [timer] scan data section"};
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
        if(wasmmod.datasec.sec_begin) [[unlikely]]
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
                                u8"Duplicate WASM Section: Data."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        wasmmod.datasec.sec_begin = begin;
        wasmmod.datasec.sec_end = end;

        // curr
        auto curr{begin};

        // get data size
        ::std::size_t data_count{};
        auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                  reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                  ::fast_io::mnp::leb128_get(data_count))};
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
                                u8"Invalid table length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }
        }

        // check 64 index
        ::uwvm::wasm::check_index(data_count);

        wasmmod.datasec.count = data_count;
        wasmmod.datasec.entries.clear();
        wasmmod.datasec.entries.reserve(data_count);

        // jump to data type
        curr = reinterpret_cast<::std::byte const*>(next);

        ::std::size_t data_counter{};

        auto const memory_count{wasmmod.importsec.memory_types.size() + wasmmod.memorysec.memory_count};

        for(; curr < end;)
        {
            ::uwvm::wasm::data_segment ds{};  // union

            if(++data_counter > data_count) [[unlikely]]
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
                                u8"The number of data segments does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            // index
            ::std::size_t index{};
            auto const [next_index, err_index]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid table length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }

            if(index >= memory_count) [[unlikely]]
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
                                u8"Invalid memory index."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            ds.index = index;

            curr = reinterpret_cast<::std::byte const*>(next_index);

            // init_expr
            ::uwvm::wasm::op_basic_type opb{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(opb), curr, sizeof(::uwvm::wasm::op_basic_type));
            ds.offset.type_opcode.op = static_cast<::uwvm::wasm::op_basic>(opb);

            if(static_cast<::uwvm::wasm::op_basic>(opb) != ::uwvm::wasm::op_basic::i32_const) [[unlikely]]
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
                                u8"Invalid i32 initializer expression that calculates the offset of a placement element."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            // set curr to i32 value
            ++curr;

            ::std::int_least32_t i32val{};
            auto const [next_i32, err_i32]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                              reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                              ::fast_io::mnp::leb128_get(i32val))};
            switch(err_i32)
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
                                u8"Invalid varint32."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }
            ds.offset.i32 = i32val;

            curr = reinterpret_cast<::std::byte const*>(next_i32);

            // op terminator
            ::std::uint_least8_t op_terminator{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(op_terminator), curr, sizeof(::std::uint_least8_t));
            if(op_terminator != 0x0b) [[unlikely]]
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
                                u8"No terminator found."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }
            ++curr;

            // size
            ::std::size_t size{};
            auto const [next_size, err_size]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                ::fast_io::mnp::leb128_get(size))};
            switch(err_size)
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
                                u8"Invalid size length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
            }

            // check 64-bit indexes
            ::uwvm::wasm::check_index(size);
            ds.size = size;

            curr = reinterpret_cast<::std::byte const*>(next_size);

            ds.data_begin = curr;
            curr += size;
            ds.data_end = curr;

            wasmmod.datasec.entries.push_back_unchecked(ds);
        }

        if(data_counter != data_count) [[unlikely]]
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
                                u8"The number of dates resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
    }
}  // namespace uwvm::wasm
