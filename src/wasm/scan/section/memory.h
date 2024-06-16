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
    inline void scan_memory_section(::uwvm::wasm::wasm_module& wasmmod, ::std::byte const* begin, ::std::byte const* end) noexcept
    {
#ifdef UWVM_TIMER
        ::fast_io::timer scan_memory_section_timer{u8"uwvm: [timer] scan memory section"};
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
        if(wasmmod.memorysec.sec_begin) [[unlikely]]
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
                                u8"Duplicate WASM Section: Memory."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }

        wasmmod.memorysec.sec_begin = begin;
        wasmmod.memorysec.sec_end = end;

        // curr
        auto curr{begin};

        // get function size
        ::std::size_t mem_count{};
        auto const [next, err]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                        reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                        ::fast_io::mnp::leb128_get(mem_count))};
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

        if(mem_count > 1) [[unlikely]]
        {
            if(::uwvm::features::enable_multi_memory) { ::uwvm::wasm::check_index(mem_count); }
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"In the MVP, the number of memories must be no more than 1."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }
        }

        wasmmod.memorysec.memory_count = mem_count;
        wasmmod.memorysec.types.clear();
        wasmmod.memorysec.types.reserve(mem_count);

        // jump to memory type
        curr = reinterpret_cast<::std::byte const*>(next);
        ::std::size_t mem_counter{};

        for(::uwvm::wasm::memory_type mt{}; curr < end;)
        {
            if(++mem_counter > mem_count) [[unlikely]]
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
                                u8"The number of memories resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            // get flags
            ::std::uint_fast8_t flags{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(flags), curr, sizeof(::std::uint_fast8_t));

            if(auto const flags0b{flags & 0x01}; flags0b == 0)
            {
                mt.limits.present_max = false;

                ++curr;

                // get type index
                ::std::size_t limit_min{};
                auto const [next_lmin, err_lmin]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
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
                                u8"(offset=",
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
                mt.limits.min = limit_min;

                curr = reinterpret_cast<::std::byte const*>(next_lmin);
            }
            else if(flags0b == 1)
            {
                mt.limits.present_max = true;

                ++curr;

                // get type index
                ::std::size_t limit_min{};
                auto const [next_lmin, err_lmin]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Invalid limit length."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                            ::fast_io::fast_terminate();
                        }
                }

                mt.limits.min = limit_min;

                curr = reinterpret_cast<::std::byte const*>(next_lmin);

                ::std::size_t limit_max{};
                auto const [next_lmax, err_lmax]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
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
                                u8"(offset=",
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
                                u8"(offset=",
                                ::fast_io::mnp::addrvw(curr - wasmmod.module_begin),
                                u8") "
                                u8"Initial > Maximum."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }

                mt.limits.max = limit_max;

                curr = reinterpret_cast<::std::byte const*>(next_lmax);
            }

            auto const flags1b{(flags & 0x02) >> 1};

            if(flags1b == 1 && !::uwvm::features::enable_thread) [[unlikely]]
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
                                u8"Whether the memory is shared or unshared. This was introduced in the threads proposal(--enable-thread)."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            mt.is_shared = static_cast<bool>(flags1b);

            auto const flags2b{(flags & 0x04) >> 2};

            if(flags2b == 1 && !::uwvm::features::enable_memory64) [[unlikely]]
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
                                u8"The index type of memory is i64, memory64 feature is required."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            auto const flags3b{(flags & 0x08) >> 3};

            if(flags3b == 1)
            {
                if(!::uwvm::features::enable_custom_page_sizes) [[unlikely]]
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
                                u8"The index type of memory is i64, memory64 feature is required."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }

                ::std::uint_fast8_t pagesize{};

                auto const [next_pagesize, err_pagesize]{::fast_io::parse_by_scan(reinterpret_cast<char8_t_const_may_alias_ptr>(curr),
                                                                                  reinterpret_cast<char8_t_const_may_alias_ptr>(end),
                                                                                  ::fast_io::mnp::leb128_get(pagesize))};
                switch(err_pagesize)
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
                                u8"Invalid pagesize."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                            ::fast_io::fast_terminate();
                        }
                }

                if(pagesize > 16) [[unlikely]]
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
                                u8"The number of pagesize must be no more than 16."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }

                mt.mempagesize = static_cast<::std::size_t>(1) << pagesize;
                curr = reinterpret_cast<::std::byte const*>(next_pagesize);
            }

            if(flags & 0xF0) [[unlikely]]
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
                                u8"Invalid flags."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            wasmmod.memorysec.types.emplace_back_unchecked(mt);
        }

        if(mem_counter != mem_count) [[unlikely]]
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
                                u8"The number of memories resolved does not match the actual number."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
            ::fast_io::fast_terminate();
        }
    }
}  // namespace uwvm::wasm
