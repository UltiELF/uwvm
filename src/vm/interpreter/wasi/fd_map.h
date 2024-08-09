#pragma once

#include <fast_io.h>
#include <fast_io_dsal/vector.h>

namespace uwvm::vm::interpreter::wasi
{
#if __has_cpp_attribute(__gnu__::__packed__)
    [[__gnu__::__packed__]]
#endif
    struct wasm_fd
    {
        int fd{-1};
        ::fast_io::native_mutex* fd_mutex{};

        using Alloc = ::fast_io::native_typed_global_allocator<::fast_io::native_mutex>;

        wasm_fd() noexcept { fd_mutex = Alloc::allocate(1); }

        wasm_fd(int ofd) noexcept : fd{ofd} { fd_mutex = Alloc::allocate(1); }

        wasm_fd(wasm_fd const& other) noexcept : fd{other.fd} { fd_mutex = Alloc::allocate(1); }

        wasm_fd& operator= (wasm_fd const& other) noexcept
        {
            fd = other.fd;
            fd_mutex = Alloc::allocate(1);
            return *this;
        }

        wasm_fd(wasm_fd&& other) noexcept : fd{other.fd}, fd_mutex{other.fd_mutex}
        {
            other.fd = -1;
            other.fd_mutex = nullptr;
        }

        wasm_fd& operator= (wasm_fd&& other) noexcept
        {
            fd = other.fd;
            fd_mutex = other.fd_mutex;
            other.fd = -1;
            other.fd_mutex = nullptr;
            return *this;
        }

        ~wasm_fd() { clear(); }

        void clear() noexcept
        {
            fd = -1;
            if(fd_mutex != nullptr) [[likely]] { Alloc::deallocate_n(fd_mutex, 1); }
        }

#if 0
        operator int& () noexcept { return fd; }
#endif
    };

    struct wasm_fd_storage_t
    {
        ::fast_io::vector<wasm_fd> opens{};
        ::fast_io::vector<::std::size_t> closes{};
        ::fast_io::native_mutex fds_mutex{};
    };

    inline void init(wasm_fd_storage_t& wasm_fd_storage) noexcept
    {
        // no need to look mutex
#ifdef __MSDOS__
        wasm_fd_storage.opens.reserve(1024);
        wasm_fd_storage.closes.reserve(1024);
#else
        wasm_fd_storage.opens.reserve(32768);
        wasm_fd_storage.closes.reserve(32768);
#endif
    }

    inline wasm_fd get_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wasm_fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wasm_fd)};
        wasm_fd_storage.fds_mutex.lock();
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]]
        {
            wasm_fd_storage.fds_mutex.unlock();
            return {-1};
        }
        else
        {
            auto const ret{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            wasm_fd_storage.fds_mutex.unlock();
            return ret;
        }
    }

    inline bool reset_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wasm_fd, int fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wasm_fd)};
        wasm_fd_storage.fds_mutex.lock();
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]]
        {
            wasm_fd_storage.fds_mutex.unlock();
            return false;
        }
        wasm_fd_storage.opens.index_unchecked(wasm_fd_pos) = fd;
        wasm_fd_storage.fds_mutex.unlock();
        return true;
    }

    inline ::std::int_least32_t create_wasm_fd(wasm_fd_storage_t& wasm_fd_storage, int fd) noexcept
    {
        wasm_fd_storage.fds_mutex.lock();
        if(wasm_fd_storage.closes.empty())
        {
            auto const pos_p{__builtin_addressof(wasm_fd_storage.opens.emplace_back(fd))};
            auto const ret{static_cast<::std::int_least32_t>(pos_p - wasm_fd_storage.opens.begin())};
            wasm_fd_storage.fds_mutex.unlock();
            return ret;
        }
        else
        {
            auto const pos{wasm_fd_storage.closes.back_unchecked()};
            wasm_fd_storage.closes.pop_back_unchecked();
            wasm_fd_storage.opens.index_unchecked(pos) = fd;
            wasm_fd_storage.fds_mutex.unlock();
            return static_cast<::std::int_least32_t>(pos);
        }
    }

    inline bool delete_wasm_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wasm_fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wasm_fd)};
        wasm_fd_storage.fds_mutex.lock();
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]]
        {
            wasm_fd_storage.fds_mutex.unlock();
            return false;
        }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check.fd == -1) [[unlikely]]
            {
                wasm_fd_storage.fds_mutex.unlock();
                return false;
            }
            fd_need_check = -1;
            wasm_fd_storage.closes.push_back(wasm_fd_pos);
            wasm_fd_storage.fds_mutex.unlock();
            return true;
        }
    }

    inline wasm_fd_storage_t wasm_fd_storages{};

}  // namespace uwvm::vm::interpreter::wasi

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::interpreter::wasi::wasm_fd_storage_t>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::interpreter::wasi::wasm_fd_storage_t>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding

