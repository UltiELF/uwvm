#pragma once
#include "../../src/vm/memory/memory.h"
#include "../../src/vm/wasi/wasi.h"

namespace uwvm::test
{
    inline void test_wasi() noexcept
    {
        ::fast_io::perr(::uwvm::u8out, u8"Test: WASI\n");

        ::fast_io::perr(::uwvm::u8out, u8"Initialize memory for WASI\n");
        auto& mem{::uwvm::vm::memories.emplace_back()};
        mem.init_by_memory_type(::uwvm::wasm::memory_type{
            .limits{.min{10}, .max{1000}, .present_max{true}}
        });

        {
            ::fast_io::perr(::uwvm::u8out, u8"args_get(", 0, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::wasi::args_get(0, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"return=", res);

            if(res != 0) [[unlikely]]
            {
                ::uwvm::test::failed = true;
                ::fast_io::perr(::uwvm::u8out, u8"\033[31mFailed\033[0m\n");
            }
            else
            {

                auto const memory_begin{::uwvm::vm::memories.front_unchecked().memory_begin};

                ::std::uint_least32_t* args_begin{reinterpret_cast<::std::uint_least32_t*>(memory_begin + 0)};
                ::std::size_t counter{};
                while(*args_begin)
                {
                    char8_t* args{reinterpret_cast<char8_t*>(memory_begin + ::fast_io::little_endian(*args_begin))};

                    ::fast_io::perrln(::uwvm::u8out, u8"arg[", counter++, u8"] ", ::fast_io::mnp::os_c_str(args));
                    ++args_begin;
                }
                ::fast_io::perr(::uwvm::u8out, u8"\033[32mSuccessfully\033[0m\n");
            }
        }

        {
            ::fast_io::perr(::uwvm::u8out, u8"args_sizes_get(", 0, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::wasi::args_sizes_get(0, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"return=", res);

            if(res != 0) [[unlikely]]
            {
                ::uwvm::test::failed = true;
                ::fast_io::perr(::uwvm::u8out, u8"\033[31mFailed\033[0m\n");
            }
            else
            {

                auto const memory_begin{::uwvm::vm::memories.front_unchecked().memory_begin};

                ::std::uint_least32_t args_sz{};
                ::fast_io::freestanding::my_memcpy(__builtin_addressof(args_sz), memory_begin, sizeof(::std::uint_least32_t));
                auto const args_sz_le{::fast_io::little_endian(args_sz)};

                ::std::uint_least32_t args_length{};
                ::fast_io::freestanding::my_memcpy(__builtin_addressof(args_length), memory_begin + 1024, sizeof(::std::uint_least32_t));
                auto const args_length_le{::fast_io::little_endian(args_length)};

                ::fast_io::perrln(::uwvm::u8out, u8"args_sz=", args_sz_le, u8", args_length=", args_length_le);

                ::fast_io::perr(::uwvm::u8out, u8"\033[32mSuccessfully\033[0m\n");
            }
        }

        {
            ::fast_io::perr(::uwvm::u8out, u8"environ_get(", 0, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::wasi::environ_get(0, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"return=", res);

            if(res != 0) [[unlikely]]
            {
                ::uwvm::test::failed = true;
                ::fast_io::perr(::uwvm::u8out, u8"\033[31mFailed\033[0m\n");
            }
            else
            {
                auto const memory_begin{::uwvm::vm::memories.front_unchecked().memory_begin};

                ::std::uint_least32_t* args_begin{reinterpret_cast<::std::uint_least32_t*>(memory_begin + 0)};
                ::std::size_t counter{};
                while(*args_begin)
                {
                    char8_t* args{reinterpret_cast<char8_t*>(memory_begin + ::fast_io::little_endian(*args_begin))};

                    ::fast_io::perrln(::uwvm::u8out, u8"env[", counter++, u8"] ", ::fast_io::mnp::os_c_str(args));
                    ++args_begin;
                }
                ::fast_io::perr(::uwvm::u8out, u8"\033[32mSuccessfully\033[0m\n");
            }
        }

        {
            ::fast_io::perr(::uwvm::u8out, u8"environ_sizes_get(", 0, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::wasi::environ_sizes_get(0, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"return=", res);

            if(res != 0) [[unlikely]]
            {
                ::uwvm::test::failed = true;
                ::fast_io::perr(::uwvm::u8out, u8"\033[31mFailed\033[0m\n");
            }
            else
            {
                auto const memory_begin{::uwvm::vm::memories.front_unchecked().memory_begin};

                ::std::uint_least32_t args_sz{};
                ::fast_io::freestanding::my_memcpy(__builtin_addressof(args_sz), memory_begin, sizeof(::std::uint_least32_t));
                auto const args_sz_le{::fast_io::little_endian(args_sz)};

                ::std::uint_least32_t args_length{};
                ::fast_io::freestanding::my_memcpy(__builtin_addressof(args_length), memory_begin + 1024, sizeof(::std::uint_least32_t));
                auto const args_length_le{::fast_io::little_endian(args_length)};

                ::fast_io::perrln(::uwvm::u8out, u8"environ_sz=", args_sz_le, u8", environ_length=", args_length_le);

                ::fast_io::perr(::uwvm::u8out, u8"\033[32mSuccessfully\033[0m\n");
            }
        }

        {
            ::fast_io::perr(::uwvm::u8out, u8"clock_res_get(", 0, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::wasi::clock_res_get(0, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"clock_res_get=", res);

            if(res != 0) [[unlikely]]
            {
                ::uwvm::test::failed = true;
                ::fast_io::perr(::uwvm::u8out, u8"\033[31mFailed\033[0m\n");
            }
            else
            {
                auto const memory_begin{::uwvm::vm::memories.front_unchecked().memory_begin};

                ::std::uint_least64_t ts{};
                ::fast_io::freestanding::my_memcpy(__builtin_addressof(ts), memory_begin + 1024, sizeof(::std::uint_least64_t));
                auto const ts_le{::fast_io::little_endian(ts)};

                ::fast_io::perrln(::uwvm::u8out, u8"resolution=", ts_le);

                ::fast_io::perr(::uwvm::u8out, u8"\033[32mSuccessfully\033[0m\n");
            }
        }

        {
            ::fast_io::perr(::uwvm::u8out, u8"clock_time_get(", 0, u8",", 512, u8",", 1024, u8")\n");
            auto const res{uwvm::vm::wasi::clock_time_get(0, 512, 1024)};
            ::fast_io::perrln(::uwvm::u8out, u8"clock_res_get=", res);

            if(res != 0) [[unlikely]]
            {
                ::uwvm::test::failed = true;
                ::fast_io::perr(::uwvm::u8out, u8"\033[31mFailed\033[0m\n");
            }
            else
            {
                auto const memory_begin{::uwvm::vm::memories.front_unchecked().memory_begin};

                ::std::uint_least64_t pre{};
                ::fast_io::freestanding::my_memcpy(__builtin_addressof(pre), memory_begin + 512, sizeof(::std::uint_least64_t));
                auto const pre_le{::fast_io::little_endian(pre)};

                ::std::uint_least64_t ts{};
                ::fast_io::freestanding::my_memcpy(__builtin_addressof(ts), memory_begin + 1024, sizeof(::std::uint_least64_t));
                auto const ts_le{::fast_io::little_endian(ts)};

                ::fast_io::perrln(::uwvm::u8out, u8"precision=", pre_le, u8", time=", ts_le);

                ::fast_io::perr(::uwvm::u8out, u8"\033[32mSuccessfully\033[0m\n");
            }
        }
    }
}  // namespace uwvm::test
