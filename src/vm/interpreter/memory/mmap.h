#pragma once
#include <bit>
#include <sys/mman.h>
#include <sys/resource.h>
#include <fast_io.h>
#include <io_device.h>
#include <instrinsic.h>

#include "../../../wasm/memlimit.h"
#include "../../../run/wasm_file.h"
#include "../../../run/features.h"

namespace uwvm::vm::interpreter::memory
{
    namespace details
    {
#if __has_cpp_attribute(__gnu__::__const__)
        [[__gnu__::__const__]]
#endif
        inline ::std::size_t
            get_max_page_size_lg2() noexcept
        {
            auto const preferred_virtual_page_size{static_cast<::std::uint_least32_t>(::fast_io::noexcept_call(::sysconf, _SC_PAGESIZE))};
            // Verify our assumption that the virtual page size is a power of two.
            return static_cast<::std::size_t>(::uwvm::instrinsic::floor_log2(preferred_virtual_page_size));
        }

        inline ::std::size_t get_static_max_page_size_lg2() noexcept
        {
            static auto const mps{get_max_page_size_lg2()};
            return mps;
        }

    }  // namespace details

    struct memory_t
    {
        ::std::byte* memory_begin{};
        ::std::size_t memory_length{};
        ::std::size_t total_pages{};

        inline static ::fast_io::native_mutex mutex{};
        inline static constexpr ::std::size_t page_size{::uwvm::wasm::num_bytes_per_page};
        inline static ::std::size_t const system_page_size{details::get_static_max_page_size_lg2()};

        memory_t() noexcept = default;

        memory_t(::uwvm::wasm::memory_type const& msec) noexcept { init_by_memory_type(msec); }

        void init_by_memory_type(::uwvm::wasm::memory_type const& msec) noexcept
        {
            mutex.lock();
            auto const mpslg2{system_page_size};

            memory_length = msec.limits.min << mpslg2;

            ::std::size_t memory_max_pages{};
            if(::uwvm::features::enable_memory64)
            {
                memory_max_pages = ::std::min(msec.limits.min, ::uwvm::wasm::max_memory64_wasm_pages);

                memory_max_pages <<= ::uwvm::wasm::num_bytes_per_page_log2 - mpslg2;
            }
            else
            {
                if constexpr(sizeof(::std::size_t) == 8) { memory_max_pages = (static_cast<::std::uint_fast64_t>(8) * 1024 * 1024 * 1024) >> mpslg2; }
                else { memory_max_pages = (static_cast<::std::uint_fast64_t>(2) * 1024 * 1024 * 1024) >> mpslg2; }
            }
            constexpr ::std::size_t memory_num_guard_bytes{65536};
            auto const num_guard_pages = memory_num_guard_bytes >> mpslg2;
            total_pages = memory_max_pages + num_guard_pages;

            memory_begin = ::fast_io::details::sys_mmap(nullptr, total_pages, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

            ::fast_io::details::sys_mprotect(memory_begin, memory_length, PROT_READ | PROT_WRITE);

            mutex.unlock();
        }

        void grow_by_memory_type(::uwvm::wasm::memory_type const& msec, ::std::size_t sz) noexcept
        {
            if(sz == 0) [[unlikely]] { return; }

            if(memory_begin == nullptr) [[unlikely]]
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
                                u8"memory_begin == nullptr."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                ::fast_io::fast_terminate();
            }
            else
            {
                auto const max_generate = [&msec]() noexcept -> ::std::size_t
                {
                    // do sth
                    return msec.limits.max;
                };

                static auto const max{max_generate()};

                auto const mpslg2{system_page_size};

                if(sz <= max && memory_length >> system_page_size <= max - sz) [[likely]]
                {
                    mutex.lock();

                    memory_length += sz << mpslg2;

                    ::fast_io::details::sys_mprotect(memory_begin, memory_length, PROT_READ | PROT_WRITE);

                    mutex.unlock();
                }
                else
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
                                u8"Memory allocation has reached its maximum limit."
                                u8"\n"
                                u8"\033[0m"
                                u8"Terminate.\n\n");
                    ::fast_io::fast_terminate();
                }
            }
        }

        memory_t(memory_t const& other) noexcept
        {
            memory_length = other.memory_length;

            auto const mpslg2{system_page_size};

            ::std::size_t memory_max_pages{};
            if(::uwvm::features::enable_memory64)
            {
                memory_max_pages = ::std::max(memory_length >> mpslg2, (static_cast<::std::uint_fast64_t>(8) * 1024 * 1024 * 1024) >> mpslg2);
                memory_max_pages <<= ::uwvm::wasm::num_bytes_per_page_log2 - mpslg2;
            }
            else
            {
                if constexpr(sizeof(::std::size_t) == 8) { memory_max_pages = (static_cast<::std::uint_fast64_t>(8) * 1024 * 1024 * 1024) >> mpslg2; }
                else { memory_max_pages = (static_cast<::std::uint_fast64_t>(2) * 1024 * 1024 * 1024) >> mpslg2; }
            }
            total_pages = memory_max_pages;
            memory_begin = ::fast_io::details::sys_mmap(nullptr, memory_max_pages, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

            ::fast_io::details::sys_mprotect(memory_begin, memory_length, PROT_READ | PROT_WRITE);

            ::fast_io::freestanding::non_overlapped_copy_n(other.memory_begin, memory_length, memory_begin);
        }

        memory_t& operator= (memory_t const& other) noexcept
        {
            mutex.lock();

            clean();
            memory_length = other.memory_length;

            auto const mpslg2{system_page_size};

            ::std::size_t memory_max_pages{};
            if(::uwvm::features::enable_memory64)
            {
                memory_max_pages = ::std::max(memory_length >> mpslg2, (static_cast<::std::uint_fast64_t>(8) * 1024 * 1024 * 1024) >> mpslg2);
                memory_max_pages <<= ::uwvm::wasm::num_bytes_per_page_log2 - mpslg2;
            }
            else
            {
                if constexpr(sizeof(::std::size_t) == 8) { memory_max_pages = (static_cast<::std::uint_fast64_t>(8) * 1024 * 1024 * 1024) >> mpslg2; }
                else { memory_max_pages = (static_cast<::std::uint_fast64_t>(2) * 1024 * 1024 * 1024) >> mpslg2; }
            }
            total_pages = memory_max_pages;
            memory_begin = ::fast_io::details::sys_mmap(nullptr, memory_max_pages, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

            ::fast_io::details::sys_mprotect(memory_begin, memory_length, PROT_READ | PROT_WRITE);

            ::fast_io::freestanding::non_overlapped_copy_n(other.memory_begin, memory_length, memory_begin);

            mutex.unlock();

            return *this;
        }

        memory_t(memory_t&& other) noexcept
        {
            memory_length = other.memory_length;
            memory_begin = other.memory_begin;
            total_pages = other.total_pages;

            other.memory_length = 0;
            other.memory_begin = nullptr;
            total_pages = 0;
        }

        memory_t& operator= (memory_t&& other) noexcept
        {
            clean();
            memory_length = other.memory_length;
            memory_begin = other.memory_begin;
            total_pages = other.total_pages;

            other.memory_length = 0;
            other.memory_begin = nullptr;
            total_pages = 0;

            return *this;
        }

        void clean() noexcept
        {
            if(memory_begin) [[likely]]
            {
                if(::fast_io::details::sys_munmap(memory_begin, total_pages)) [[unlikely]] { ::fast_io::fast_terminate(); }
                memory_length = 0;
                memory_begin = nullptr;
                total_pages = 0;

            }
        }

        ~memory_t() { clean(); }
    };

}  // namespace uwvm::vm::interpreter::memory

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::interpreter::memory::memory_t>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::interpreter::memory::memory_t>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
