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
    inline void scan_import_section(::uwvm::wasm::wasm_module& wasmmod, ::std::byte const* begin, ::std::byte const* end) noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer scan_import_section_timer{u8"uwvm: [timer] scan import section"};
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

        if(!wasmmod.typesec.sec_begin) [[unlikely]]
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
                                u8"The type section must appear before import section."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        // check is exist
        if(wasmmod.importsec.sec_begin) [[unlikely]]
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
                                u8"Duplicate WASM Section: Import."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
        wasmmod.importsec.sec_begin = begin;
        wasmmod.importsec.sec_end = end;

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
                                u8"(",
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
        ::uwvm::wasm::check_index(import_count);

        wasmmod.importsec.import_count = import_count;
        wasmmod.importsec.types.clear();
        wasmmod.importsec.types.reserve(import_count);

        // jump to 1st import leb128
        curr = reinterpret_cast<::std::byte const*>(next);

        ::std::size_t import_counter{};

        ::std::size_t import_func_counter{};
        ::std::size_t import_table_counter{};
        ::std::size_t import_memory_counter{};
        ::std::size_t import_global_counter{};
        ::std::size_t import_tag_counter{};

        auto const type_section_cbegin{wasmmod.typesec.types.cbegin()};
        auto const type_section_count{wasmmod.typesec.type_count};
        for(; curr < end;)
        {
            ::uwvm::wasm::section::import_type it{};  // has union

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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
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
                                u8"(",
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
            ::uwvm::wasm::check_index(module_len);

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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
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
                                u8"(",
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
            ::uwvm::wasm::check_index(name_len);

            if(name_len == 0) [[unlikely]]
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
                                u8"Invalid name length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid import name length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            ::uwvm::wasm::extern_kind ek{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(ek), curr, sizeof(::uwvm::wasm::extern_kind));
            if(!::uwvm::wasm::is_valid_extern_kind_type(ek)) [[unlikely]]
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

            // set curr
            ++curr;

            it.extern_type.type = ek;

            switch(ek)
            {
                case ::uwvm::wasm::extern_kind::func:
                {
                    ++import_func_counter;

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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid type length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    // check
                    if(type_index >= type_section_count) [[unlikely]]
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
                                u8"Invalid type index."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    it.extern_type.function = type_section_cbegin + type_index;

                    // set curr
                    curr = reinterpret_cast<::std::byte const*>(next_ti);

                    break;
                }
                case ::uwvm::wasm::extern_kind::table:
                {
                    ++import_table_counter;

                    ::uwvm::wasm::value_type et{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(et), curr, sizeof(::uwvm::wasm::value_type));

                    if(!::uwvm::wasm::is_valid_value_type_ref(et)) [[unlikely]]
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
                                u8"Invalid Elem Type: ",
                                ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(et)),
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    it.extern_type.table.elem_type = et;

                    // jump to flags
                    ++curr;

                    // get flags
                    ::std::uint_fast8_t flags{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(flags), curr, sizeof(::std::uint_fast8_t));

                    if(flags == 0)
                    {
                        it.extern_type.table.limits.present_max = static_cast<bool>(flags);

                        ++curr;

                        // get type index
                        ::std::size_t limit_min{};
                        auto [next_lmin, err_lmin]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                            reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                            ::fast_io::mnp::leb128_get(limit_min))};
                        switch(err_lmin)
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
                                            u8"Invalid limit length."
                                            u8"\n"
                                            u8"\033[0m"
                                            u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                        }

                        // check 64-bit indexes
                        ::uwvm::wasm::check_index(limit_min);
                        it.extern_type.table.limits.min = limit_min;

                        curr = reinterpret_cast<::std::byte const*>(next_lmin);
                    }
                    else if(flags == 1)
                    {
                        it.extern_type.table.limits.present_max = static_cast<bool>(flags);

                        ++curr;

                        // get type index
                        ::std::size_t limit_min{};
                        auto [next_lmin, err_lmin]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                            reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                            ::fast_io::mnp::leb128_get(limit_min))};
                        switch(err_lmin)
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
                                            u8"Invalid limit length."
                                            u8"\n"
                                            u8"\033[0m"
                                            u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                        }

                        it.extern_type.table.limits.min = limit_min;

                        curr = reinterpret_cast<::std::byte const*>(next_lmin);

                        ::std::size_t limit_max{};
                        auto [next_lmax, err_lmax]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                            reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                            ::fast_io::mnp::leb128_get(limit_max))};
                        switch(err_lmax)
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
                                            u8"Invalid limit length."
                                            u8"\n"
                                            u8"\033[0m"
                                            u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                        }

                        // check 64-bit indexes
                        ::uwvm::wasm::check_index(limit_max);

                        if(limit_min > limit_max) [[unlikely]]
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
                                u8"Initial > Maximum."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                            ::fast_io::fast_terminate();
                        }

                        it.extern_type.table.limits.max = limit_max;

                        curr = reinterpret_cast<::std::byte const*>(next_lmax);
                    }
                    else [[unlikely]]
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
                                u8"Invalid flags."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
                    break;
                }
                case ::uwvm::wasm::extern_kind::memory:
                {
                    ++import_memory_counter;

                    // get flags
                    ::std::uint_fast8_t flags{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(flags), curr, sizeof(::std::uint_fast8_t));

                    if(flags == 0)
                    {
                        it.extern_type.memory.limits.present_max = static_cast<bool>(flags);

                        ++curr;

                        // get type index
                        ::std::size_t limit_min{};
                        auto [next_lmin, err_lmin]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                            reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                            ::fast_io::mnp::leb128_get(limit_min))};
                        switch(err_lmin)
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
                                            u8"Invalid limit length."
                                            u8"\n"
                                            u8"\033[0m"
                                            u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                        }

                        // check 64-bit indexes
                        ::uwvm::wasm::check_index(limit_min);
                        it.extern_type.memory.limits.min = limit_min;

                        curr = reinterpret_cast<::std::byte const*>(next_lmin);
                    }
                    else if(flags == 1)
                    {
                        it.extern_type.memory.limits.present_max = static_cast<bool>(flags);

                        ++curr;

                        // get type index
                        ::std::size_t limit_min{};
                        auto [next_lmin, err_lmin]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                            reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                            ::fast_io::mnp::leb128_get(limit_min))};
                        switch(err_lmin)
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
                                            u8"Invalid limit length."
                                            u8"\n"
                                            u8"\033[0m"
                                            u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                        }

                        it.extern_type.memory.limits.min = limit_min;

                        curr = reinterpret_cast<::std::byte const*>(next_lmin);

                        ::std::size_t limit_max{};
                        auto [next_lmax, err_lmax]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                            reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                            ::fast_io::mnp::leb128_get(limit_max))};
                        switch(err_lmax)
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
                                            u8"Invalid limit length."
                                            u8"\n"
                                            u8"\033[0m"
                                            u8"Terminate.\n\n");
                                    ::fast_io::fast_terminate();
                                }
                        }

                        // check 64-bit indexes
                        ::uwvm::wasm::check_index(limit_max);

                        if(limit_min > limit_max) [[unlikely]]
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
                                u8"Initial > Maximum."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                            ::fast_io::fast_terminate();
                        }

                        it.extern_type.memory.limits.max = limit_max;

                        curr = reinterpret_cast<::std::byte const*>(next_lmax);
                    }
                    else [[unlikely]]
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
                                u8"Invalid flags."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
                    break;
                }
                case ::uwvm::wasm::extern_kind::global:
                {
                    ++import_global_counter;

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
                                        u8"(",
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
                                        u8"(",
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
                                        u8"(",
                                        ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                        u8") "
                                        u8"Invalid Value Type: ",
                                        ::fast_io::mnp::hex0x<true>(static_cast<::std::uint_fast8_t>(vt)),
                                        u8"\n"
                                        u8"\033[0m"
                                        u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    it.extern_type.global.type = vt;

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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Enter parameter --enable-mutable-globals to enable import or export mutable globals."
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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid varuint1."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }

                    it.extern_type.global.is_mutable = static_cast<bool>(ma);

                    ++curr;
                    break;
                }
                case ::uwvm::wasm::extern_kind::tag:
                {
                    ++import_tag_counter;

                    ::std::uint_least8_t att{};
                    ::fast_io::freestanding::my_memcpy(__builtin_addressof(att), curr, sizeof(::std::uint_least8_t));

                    it.extern_type.exception.attribute = att;
                    ++curr;

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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid type length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                                ::fast_io::fast_terminate();
                            }
                    }

                    // check
                    if(type_index >= type_section_count) [[unlikely]]
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
                                u8"Invalid type length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                        ::fast_io::fast_terminate();
                    }
                    it.extern_type.exception.type = type_section_cbegin + type_index;

                    break;
                }
                default: ::fast_io::unreachable();
            }

            wasmmod.importsec.types.push_back_unchecked(it);
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
                                u8"(",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"The number of imports resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        // clear
        wasmmod.importsec.func_types.clear();
        wasmmod.importsec.table_types.clear();
        wasmmod.importsec.memory_types.clear();
        wasmmod.importsec.global_types.clear();
        wasmmod.importsec.tag_types.clear();

        // reserve
        wasmmod.importsec.func_types.reserve(import_func_counter);
        wasmmod.importsec.table_types.reserve(import_table_counter);
        wasmmod.importsec.memory_types.reserve(import_memory_counter);
        wasmmod.importsec.global_types.reserve(import_global_counter);
        wasmmod.importsec.tag_types.reserve(import_tag_counter);

        for(auto const& i: wasmmod.importsec.types)
        {
            switch(i.extern_type.type)
            {
                case ::uwvm::wasm::extern_kind::func:
                {
                    wasmmod.importsec.func_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case ::uwvm::wasm::extern_kind::table:
                {
                    wasmmod.importsec.table_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case ::uwvm::wasm::extern_kind::memory:
                {
                    wasmmod.importsec.memory_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case ::uwvm::wasm::extern_kind::global:
                {
                    wasmmod.importsec.global_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                case ::uwvm::wasm::extern_kind::tag:
                {
                    wasmmod.importsec.tag_types.push_back_unchecked(__builtin_addressof(i));
                    break;
                }
                default: ::fast_io::unreachable();
            }
        }
    }
}  // namespace uwvm::wasm
