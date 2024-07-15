#pragma once
#include "../../src/vm/interpreter/memory/memory.h"
#include "../../src/vm/interpreter/wasi/wasi.h"

namespace uwvm::test
{
    inline void test_wasi() noexcept
    {
        ::fast_io::perr(::uwvm::u8out, u8"Test: WASI\n");

        ::fast_io::perr(::uwvm::u8out, u8"Initialize memory for WASI\n");
        auto& mem{::uwvm::vm::interpreter::memories.emplace_back()};
        mem.init_by_memory_type(::uwvm::wasm::memory_type{
            .limits{.min{10}, .max{1000}, .present_max{true}}
        });

        {
            ::fast_io::perr(::uwvm::u8out, u8"args_get(", 0, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::interpreter::wasi::args_get(0, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"return=", res);

            if(res != 0) [[unlikely]]
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
                                u8"Test Failed."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            auto const memory_begin{::uwvm::vm::interpreter::memories.front_unchecked().memory_begin};

            ::std::uint_least32_t* args_begin{reinterpret_cast<::std::uint_least32_t*>(memory_begin + 0)};
            ::std::size_t counter{};
            while(*args_begin)
            {
                char8_t* args{reinterpret_cast<char8_t*>(memory_begin + ::fast_io::little_endian(*args_begin))};

                ::fast_io::perrln(::uwvm::u8out, u8"arg[", counter++, u8"] ", ::fast_io::mnp::os_c_str(args));
                ++args_begin;
            }
            ::fast_io::perr(::uwvm::u8out, u8"Successfully\n");
        }

        {
            ::fast_io::perr(::uwvm::u8out, u8"args_sizes_get(", 0, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::interpreter::wasi::args_sizes_get(0, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"return=", res);

            if(res != 0) [[unlikely]]
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
                                u8"Test Failed."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            auto const memory_begin{::uwvm::vm::interpreter::memories.front_unchecked().memory_begin};

            ::std::uint_least32_t args_sz{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(args_sz), memory_begin, sizeof(::std::uint_least32_t));
            auto const args_sz_le{::fast_io::little_endian(args_sz)};

            ::std::uint_least32_t args_length{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(args_length), memory_begin + 1024, sizeof(::std::uint_least32_t));
            auto const args_sz_le{::fast_io::little_endian(args_length)};

            ::fast_io::perrln(::uwvm::u8out, u8"args_sz=", args_sz, u8", args_length=", args_length);

            ::fast_io::perr(::uwvm::u8out, u8"Successfully\n");
        }

        {
            ::fast_io::perr(::uwvm::u8out, u8"environ_get(", 0, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::interpreter::wasi::environ_get(0, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"return=", res);

            if(res != 0) [[unlikely]]
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
                                u8"Test Failed."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            auto const memory_begin{::uwvm::vm::interpreter::memories.front_unchecked().memory_begin};

            ::std::uint_least32_t* args_begin{reinterpret_cast<::std::uint_least32_t*>(memory_begin + 0)};
            ::std::size_t counter{};
            while(*args_begin)
            {
                char8_t* args{reinterpret_cast<char8_t*>(memory_begin + ::fast_io::little_endian(*args_begin))};

                ::fast_io::perrln(::uwvm::u8out, u8"arg[", counter++, u8"] ", ::fast_io::mnp::os_c_str(args));
                ++args_begin;
            }
            ::fast_io::perr(::uwvm::u8out, u8"Successfully\n");
        }

        {
            ::fast_io::perr(::uwvm::u8out, u8"environ_sizes_get(", 0, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::interpreter::wasi::args_sizes_get(0, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"return=", res);

            if(res != 0) [[unlikely]]
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
                                u8"Test Failed."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }

            auto const memory_begin{::uwvm::vm::interpreter::memories.front_unchecked().memory_begin};

            ::std::uint_least32_t args_sz{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(args_sz), memory_begin, sizeof(::std::uint_least32_t));
            auto const args_sz_le{::fast_io::little_endian(args_sz)};

            ::std::uint_least32_t args_length{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(args_length), memory_begin + 1024, sizeof(::std::uint_least32_t));
            auto const args_sz_le{::fast_io::little_endian(args_length)};

            ::fast_io::perrln(::uwvm::u8out, u8"args_sz=", args_sz, u8", args_length=", args_length);

            ::fast_io::perr(::uwvm::u8out, u8"Successfully\n");
        }
    }
}  // namespace uwvm::test
