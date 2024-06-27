#include <bit>
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
        template <bool zw>
#if __has_cpp_attribute(__gnu__::__const__)
        [[__gnu__::__const__]]
#endif
        inline ::std::size_t
            get_max_page_size_lg2() noexcept
        {
            ::fast_io::win32::nt::system_basic_information si{};
            auto const status{::fast_io::win32::nt::nt_query_system_information<zw>(::fast_io::win32::nt::system_information_class::SystemBasicInformation,
                                                                                    __builtin_addressof(si),
                                                                                    sizeof(si),
                                                                                    nullptr)};
            if(status) [[unlikely]]
            {
                ::fast_io::io::perrln( uwvm::u8out,::fast_io::error{::fast_io::nt_domain_value, status});
                constexpr ::std::size_t ps{::uwvm::instrinsic::floor_log2(static_cast<::std::size_t>(4096))};
                return ps;
            }
            return ::uwvm::instrinsic::floor_log2(si.PageSize);
        }

        template <bool zw>
        inline ::std::size_t get_static_max_page_size_lg2() noexcept
        {
            static auto const mps{get_max_page_size_lg2<zw>()};
            return mps;
        }

    }  // namespace details

    template <::fast_io::nt_family family>
    struct nt_family_memory_t
    {
        inline static constexpr bool zw{family == ::fast_io::nt_family::zw};

        ::std::byte* memory_begin{};
        ::std::size_t memory_length{};

        inline static ::fast_io::native_mutex mutex{};
        inline static constexpr ::std::size_t page_size{::uwvm::wasm::num_bytes_per_page};
        inline static ::std::size_t const system_page_size{details::get_static_max_page_size_lg2<zw>()};

        nt_family_memory_t() noexcept = default;

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
            auto total_pages{memory_max_pages + num_guard_pages};

            ::std::byte* vamemory{};
            auto status{::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                             reinterpret_cast<void**>(__builtin_addressof(vamemory)),
                                                                             0,
                                                                             __builtin_addressof(total_pages),
                                                                             0x00002000 /*MEM_RESERVE*/,
                                                                             0x01 /*PAGE_NOACCESS*/)};

            if(status) [[unlikely]] { ::fast_io::fast_terminate(); }
            memory_begin = vamemory;

            ::std::byte* vamemory2{memory_begin};
            status = ::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                          reinterpret_cast<void**>(__builtin_addressof(vamemory2)),
                                                                          0,
                                                                          __builtin_addressof(memory_length),
                                                                          0x00001000 /*MEM_COMMIT*/,
                                                                          0x04 /*PAGE_READWRITE*/);

            if(status || vamemory2 != memory_begin) [[unlikely]] { ::fast_io::fast_terminate(); }

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

                    ::std::byte* vamemory{memory_begin};
                    auto const status{::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                                           reinterpret_cast<void**>(__builtin_addressof(vamemory)),
                                                                                           0,
                                                                                           __builtin_addressof(memory_length),
                                                                                           0x00001000 /*MEM_COMMIT*/,
                                                                                           0x04 /*PAGE_READWRITE*/)};

                    if(status || vamemory != memory_begin) [[unlikely]] { ::fast_io::fast_terminate(); }

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

        nt_family_memory_t(nt_family_memory_t const& other) noexcept
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

            ::std::byte* vamemory{};
            auto status{::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                             reinterpret_cast<void**>(__builtin_addressof(vamemory)),
                                                                             0,
                                                                             __builtin_addressof(memory_max_pages),
                                                                             0x00002000 /*MEM_RESERVE*/,
                                                                             0x01 /*PAGE_NOACCESS*/)};

            if(status) [[unlikely]] { ::fast_io::fast_terminate(); }
            memory_begin = vamemory;

            ::std::byte* vamemory2{memory_begin};
            status = ::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                          reinterpret_cast<void**>(__builtin_addressof(vamemory2)),
                                                                          0,
                                                                          __builtin_addressof(memory_length),
                                                                          0x00001000 /*MEM_COMMIT*/,
                                                                          0x04 /*PAGE_READWRITE*/);

            if(status || vamemory2 != memory_begin) [[unlikely]] { ::fast_io::fast_terminate(); }

            ::fast_io::freestanding::non_overlapped_copy_n(other.memory_begin, memory_length, memory_begin);
        }

        nt_family_memory_t& operator= (nt_family_memory_t const& other) noexcept
        {
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

            ::std::byte* vamemory{};
            auto status{::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                             reinterpret_cast<void**>(__builtin_addressof(vamemory)),
                                                                             0,
                                                                             __builtin_addressof(memory_max_pages),
                                                                             0x00002000 /*MEM_RESERVE*/,
                                                                             0x01 /*PAGE_NOACCESS*/)};

            if(status) [[unlikely]] { ::fast_io::fast_terminate(); }
            memory_begin = vamemory;

            ::std::byte* vamemory2{memory_begin};
            status = ::fast_io::win32::nt::nt_allocate_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                          reinterpret_cast<void**>(__builtin_addressof(vamemory2)),
                                                                          0,
                                                                          __builtin_addressof(memory_length),
                                                                          0x00001000 /*MEM_COMMIT*/,
                                                                          0x04 /*PAGE_READWRITE*/);

            if(status || vamemory2 != memory_begin) [[unlikely]] { ::fast_io::fast_terminate(); }

            ::fast_io::freestanding::non_overlapped_copy_n(other.memory_begin, memory_length, memory_begin);
            return *this;
        }

        nt_family_memory_t(nt_family_memory_t&& other) noexcept
        {
            memory_length = other.memory_length;
            memory_begin = other.memory_begin;
            other.memory_length = 0;
            other.memory_begin = nullptr;
        }

        nt_family_memory_t& operator= (nt_family_memory_t&& other) noexcept
        {
            clean();
            memory_length = other.memory_length;
            memory_begin = other.memory_begin;
            other.memory_length = 0;
            other.memory_begin = nullptr;
            return *this;
        }

        void clean() noexcept
        {
            if(memory_begin) [[likely]]
            {
                ::std::byte* vfmemory{memory_begin};
                ::std::size_t free_type{0};
                if(::fast_io::win32::nt::nt_free_virtual_memory<zw>(reinterpret_cast<void*>(-1),
                                                                     reinterpret_cast<void**>(__builtin_addressof(vfmemory)),
                                                                     __builtin_addressof(free_type),
                                                                     0x00008000 /*MEM_RELEASE*/)) [[unlikely]]
                {
                    ::fast_io::fast_terminate();
                }
                memory_length = 0;
                memory_begin = nullptr;
            }
        }

        ~nt_family_memory_t() { clean(); }
    };

    using memory_t = nt_family_memory_t<::fast_io::nt_family::nt>;
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
