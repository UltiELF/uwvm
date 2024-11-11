#pragma once

#include <fast_io.h>
#include <fast_io_dsal/vector.h>
#include "fd_limit.h"

namespace uwvm::vm::wasi
{
    struct wasm_fd
    {
        using mutex = ::fast_io::native_mutex;
        using handle = int;

        // ====== for vm ======
        mutex* fd_mutex{};
        ::std::size_t close_pos{SIZE_MAX};

        // ===== wasi fd =====
        ::uwvm::vm::wasi::rights_t rights_base{static_cast<::uwvm::vm::wasi::rights_t>(-1)};
        ::uwvm::vm::wasi::rights_t rights_inherit{static_cast<::uwvm::vm::wasi::rights_t>(-1)};

        handle fd{-1};

#if defined(_WIN32) && defined(_WIN32_WINDOWS)
        ::fast_io::win9x_dir_handle win9x_dir_file{};
#endif 

        // ====== for vm ======
        // wasm_fd is created in vector (before wasm fd is used) and destroyed with the vector at the end of the wasm program, and 'vector<wasm_fd> open' only
        // grows linearly, so reference counting is not required
        bool copy_mutex{};

        using Alloc = ::fast_io::native_typed_global_allocator<mutex>;

        wasm_fd() noexcept
        {
            fd_mutex = Alloc::allocate(1);
            ::std::construct_at(fd_mutex);
        }

        wasm_fd(handle ofd) noexcept : fd{ofd}
        {
            fd_mutex = Alloc::allocate(1);
            ::std::construct_at(fd_mutex);
        }

        // shared mutex

        wasm_fd(wasm_fd const& other) noexcept :
            fd_mutex{other.fd_mutex}, fd{other.fd}, close_pos{other.close_pos}, rights_base{other.rights_base}, rights_inherit{other.rights_inherit},
            copy_mutex{true}
#if defined(_WIN32) && defined(_WIN32_WINDOWS)
            ,
            win9x_dir_file{other.win9x_dir_file}
#endif 
        {
        }

        wasm_fd& operator= (wasm_fd const& other) noexcept
        {
            fd_mutex = other.fd_mutex;
            fd = other.fd;
            close_pos = other.close_pos;
            rights_base = other.rights_base;
            rights_inherit = other.rights_inherit;
            copy_mutex = true;
#if defined(_WIN32) && defined(_WIN32_WINDOWS)
            win9x_dir_file = other.win9x_dir_file;
#endif 
            return *this;
        }

        wasm_fd(wasm_fd&& other) noexcept :
            fd{other.fd}, fd_mutex{other.fd_mutex}, close_pos{other.close_pos}, rights_base{other.rights_base}, rights_inherit{other.rights_inherit},
            copy_mutex{other.copy_mutex}
#if defined(_WIN32) && defined(_WIN32_WINDOWS)
            ,
            win9x_dir_file{::std::move(other.win9x_dir_file)}
#endif 
        {
            other.fd = -1;
            other.fd_mutex = nullptr;
            other.close_pos = SIZE_MAX;
            other.rights_base = static_cast<::uwvm::vm::wasi::rights_t>(-1);
            other.rights_inherit = static_cast<::uwvm::vm::wasi::rights_t>(-1);
            other.copy_mutex = false;
        }

        wasm_fd& operator= (wasm_fd&& other) noexcept
        {
            fd = other.fd;
            fd_mutex = other.fd_mutex;
            close_pos = other.close_pos;
            rights_base = other.rights_base;
            rights_inherit = other.rights_inherit;
            copy_mutex = other.copy_mutex;
#if defined(_WIN32) && defined(_WIN32_WINDOWS)
            win9x_dir_file = ::std::move(other.win9x_dir_file);
#endif 
            other.fd = -1;
            other.fd_mutex = nullptr;
            other.close_pos = SIZE_MAX;
            other.rights_base = static_cast<::uwvm::vm::wasi::rights_t>(-1);
            other.rights_inherit = static_cast<::uwvm::vm::wasi::rights_t>(-1);
            other.copy_mutex = false;
            return *this;
        }

        ~wasm_fd() { clear(); }

        void clear() noexcept
        {
            fd = -1;
            close_pos = SIZE_MAX;
            rights_base = static_cast<::uwvm::vm::wasi::rights_t>(-1);
            rights_inherit = static_cast<::uwvm::vm::wasi::rights_t>(-1);

            if(fd_mutex != nullptr) [[likely]]
            {
                if(!copy_mutex)
                {
                    ::std::destroy_at(fd_mutex);
                    Alloc::deallocate_n(fd_mutex, 1);
                }
                fd_mutex = nullptr;
            }
            copy_mutex = false;
#if defined(_WIN32) && defined(_WIN32_WINDOWS)
            ::fast_io::win32::details::close_win9x_dir_handle(win9x_dir_file);
#endif 
        }

    };
}  // namespace uwvm::vm::wasi

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::wasi::wasm_fd>
    {
#if defined(_WIN32) && defined(_WIN32_WINDOWS)
        inline static constexpr bool value = false;
#else
        inline static constexpr bool value = true;
#endif 
    };
}  // namespace fast_io::freestanding

namespace uwvm::vm::wasi
{

    struct wasm_fd_storage_t
    {
        ::fast_io::vector<::uwvm::vm::wasi::wasm_fd> opens{};
        ::fast_io::vector<::std::size_t> closes{};
        ::fast_io::native_mutex fds_mutex{};
    };

    inline void init(wasm_fd_storage_t& wasm_fd_storage) noexcept
    {
        auto const min_fd{::std::min(static_cast<::std::size_t>(1024), ::uwvm::vm::wasi::wasi_fd_limit)};
        wasm_fd_storage.opens.reserve(min_fd);
        wasm_fd_storage.closes.reserve(min_fd);
    }

#ifdef UWVM_ENABLE_COPYED_WASMFD
    inline ::uwvm::vm::wasi::wasm_fd get_copy_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]]
        {
            // construst new mutex
            return {};
        }
        else
        {
            // shared mutex
            auto const ret{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            return ret;
        }
    }
#endif

    inline ::uwvm::vm::wasi::wasm_fd* get_original_wasm_fd_p(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]] { return nullptr; }
        else { return wasm_fd_storage.opens.begin() + wasm_fd_pos; }
    }

    inline bool reset_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd, int fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]] { return false; }
        wasm_fd_storage.opens.index_unchecked(wasm_fd_pos).fd = fd;
        return true;
    }

    inline ::std::int_least32_t create_wasm_fd(wasm_fd_storage_t& wasm_fd_storage, int fd) noexcept
    {
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_storage.closes.empty())
        {
            if(wasm_fd_storage.opens.size() >= ::uwvm::vm::wasi::wasi_fd_limit) [[unlikely]] { return -1; }
            auto const pos_p{__builtin_addressof(wasm_fd_storage.opens.emplace_back(fd))};
            auto const ret{static_cast<::std::int_least32_t>(pos_p - wasm_fd_storage.opens.begin())};
            return ret;
        }
        else
        {
            auto const pos{wasm_fd_storage.closes.back_unchecked()};
            wasm_fd_storage.closes.pop_back_unchecked();
            wasm_fd_storage.opens.index_unchecked(pos).fd = fd;
            return static_cast<::std::int_least32_t>(pos);
        }
    }

#ifdef UWVM_ENABLE_COPYED_WASMFD
    struct create_and_get_copy_wasm_fd_t
    {
        ::std::int_least32_t wfd{};
        wasm_fd fd{};
    };

    inline create_and_get_copy_wasm_fd_t create_and_get_copy_wasm_fd(wasm_fd_storage_t& wasm_fd_storage, int fd) noexcept
    {
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_storage.closes.empty())
        {
            if(wasm_fd_storage.opens.size() >= ::uwvm::vm::wasi::wasi_fd_limit) [[unlikely]] { return {-1, {}}; }
            auto const pos_p{__builtin_addressof(wasm_fd_storage.opens.emplace_back(fd))};
            auto const ret{static_cast<::std::int_least32_t>(pos_p - wasm_fd_storage.opens.begin())};
            // shared mutex
            return {ret, *pos_p};
        }
        else
        {
            auto const pos{wasm_fd_storage.closes.back_unchecked()};
            wasm_fd_storage.closes.pop_back_unchecked();
            auto& pfd{wasm_fd_storage.opens.index_unchecked(pos)};
            pfd.fd = fd;
            // shared mutex
            return {static_cast<::std::int_least32_t>(pos), pfd};
        }
    }
#endif

    struct create_and_get_original_wasm_fd_p_t
    {
        ::std::int_least32_t wfd{};
        wasm_fd* fd{};
    };

    inline create_and_get_original_wasm_fd_p_t create_and_get_original_wasm_fd_p(wasm_fd_storage_t& wasm_fd_storage, int fd) noexcept
    {
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_storage.closes.empty())
        {
            if(wasm_fd_storage.opens.size() >= ::uwvm::vm::wasi::wasi_fd_limit) [[unlikely]] { return {-1, {}}; }
            auto pos_p{__builtin_addressof(wasm_fd_storage.opens.emplace_back(fd))};
            auto const ret{static_cast<::std::int_least32_t>(pos_p - wasm_fd_storage.opens.begin())};
            return {ret, pos_p};
        }
        else
        {
            auto const pos{wasm_fd_storage.closes.back_unchecked()};
            wasm_fd_storage.closes.pop_back_unchecked();
            auto const pfd{wasm_fd_storage.opens.begin() + pos};
            pfd->fd = fd;

            return {static_cast<::std::int_least32_t>(pos), pfd};
        }
    }

    inline bool delete_wasm_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]] { return false; }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check.fd == -1) [[unlikely]] { return false; }
            fd_need_check.fd = -1;
            auto const cpos{__builtin_addressof(wasm_fd_storage.closes.emplace_back(wasm_fd_pos))};
            fd_need_check.close_pos = static_cast<::std::size_t>(cpos - wasm_fd_storage.closes.cbegin());
            return true;
        }
    }

    struct delete_and_get_system_fd_t
    {
        bool success{};
        int fd{-1};
    };

    inline delete_and_get_system_fd_t delete_and_get_system_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]] { return {false, -1}; }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check.fd == -1) [[unlikely]] { return {false, -1}; }
            auto const temp_fd{fd_need_check.fd};
            fd_need_check.fd = -1;
            auto const cpos{__builtin_addressof(wasm_fd_storage.closes.emplace_back(wasm_fd_pos))};
            fd_need_check.close_pos = static_cast<::std::size_t>(cpos - wasm_fd_storage.closes.cbegin());
            return {true, temp_fd};
        }
    }

    inline bool set_wasm_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd, int fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_pos >= ::uwvm::vm::wasi::wasi_fd_limit) [[unlikely]] { return false; }
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos)
        {
            auto const size_temp{wasm_fd_storage.opens.size()};
            wasm_fd_storage.opens.reserve(wasm_fd_pos + 1);
            if constexpr(::fast_io::freestanding::is_zero_default_constructible_v<wasm_fd>)
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                ::fast_io::freestanding::bytes_clear_n(
                    reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr),
                    static_cast<::std::size_t>(reinterpret_cast<::std::byte*>(end) - reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr)));
                wasm_fd_storage.opens.imp.curr_ptr = end;
            }
            else
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                for(; wasm_fd_storage.opens.imp.curr_ptr != end; ++wasm_fd_storage.opens.imp.curr_ptr)
                {
                    ::std::construct_at(wasm_fd_storage.opens.imp.curr_ptr);
                }
            }

            auto const new_size{wasm_fd_storage.opens.size()};
            for(::std::size_t i{size_temp}; i < new_size - 1; i++) { wasm_fd_storage.closes.push_back(i); }

            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            fd_need_check.fd = fd;
        }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check.fd == -1)
            {
                if(fd_need_check.close_pos < wasm_fd_storage.closes.size()) [[likely]]
                {
                    wasm_fd_storage.closes.erase(wasm_fd_storage.closes.cbegin() + fd_need_check.close_pos);
                }
                fd_need_check.close_pos = SIZE_MAX;
                fd_need_check.fd = fd;
            }
            else { fd_need_check.fd = fd; }
        }

        return true;
    }

#ifdef UWVM_ENABLE_COPYED_WASMFD
    struct set_and_get_copy_wasm_fd_t
    {
        bool success{};
        wasm_fd fd{};
    };

    inline set_and_get_copy_wasm_fd_t set_and_get_copy_wasm_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd, int fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_pos >= ::uwvm::vm::wasi::wasi_fd_limit) [[unlikely]] { return {false, {}}; }
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos)
        {
            auto const size_temp{wasm_fd_storage.opens.size()};
            wasm_fd_storage.opens.reserve(wasm_fd_pos + 1);
            if constexpr(::fast_io::freestanding::is_zero_default_constructible_v<wasm_fd>)
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                ::fast_io::freestanding::bytes_clear_n(
                    reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr),
                    static_cast<::std::size_t>(reinterpret_cast<::std::byte*>(end) - reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr)));
                wasm_fd_storage.opens.imp.curr_ptr = end;
            }
            else
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                for(; wasm_fd_storage.opens.imp.curr_ptr != end; ++wasm_fd_storage.opens.imp.curr_ptr)
                {
                    ::std::construct_at(wasm_fd_storage.opens.imp.curr_ptr);
                }
            }

            auto const new_size{wasm_fd_storage.opens.size()};
            for(::std::size_t i{size_temp}; i < new_size - 1; i++) { wasm_fd_storage.closes.push_back(i); }

            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            fd_need_check.fd = fd;

            return {true, fd_need_check};
        }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check.fd == -1)
            {
                if(fd_need_check.close_pos < wasm_fd_storage.closes.size()) [[likely]]
                {
                    wasm_fd_storage.closes.erase(wasm_fd_storage.closes.cbegin() + fd_need_check.close_pos);
                }
                fd_need_check.close_pos = SIZE_MAX;
                fd_need_check.fd = fd;
            }
            else { fd_need_check.fd = fd; }

            return {true, fd_need_check};
        }
    }
#endif

    struct set_and_get_original_wasm_fd_p_t
    {
        bool success{};
        wasm_fd* fd{};
    };

    inline set_and_get_original_wasm_fd_p_t set_and_get_original_wasm_fd_p(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd, int fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};
        if(wasm_fd_pos >= ::uwvm::vm::wasi::wasi_fd_limit) [[unlikely]] { return {false, {}}; }
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos)
        {
            auto const size_temp{wasm_fd_storage.opens.size()};
            wasm_fd_storage.opens.reserve(wasm_fd_pos + 1);
            if constexpr(::fast_io::freestanding::is_zero_default_constructible_v<wasm_fd>)
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                ::fast_io::freestanding::bytes_clear_n(
                    reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr),
                    static_cast<::std::size_t>(reinterpret_cast<::std::byte*>(end) - reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr)));
                wasm_fd_storage.opens.imp.curr_ptr = end;
            }
            else
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                for(; wasm_fd_storage.opens.imp.curr_ptr != end; ++wasm_fd_storage.opens.imp.curr_ptr)
                {
                    ::std::construct_at(wasm_fd_storage.opens.imp.curr_ptr);
                }
            }

            auto const new_size{wasm_fd_storage.opens.size()};
            for(::std::size_t i{size_temp}; i < new_size - 1; i++) { wasm_fd_storage.closes.push_back(i); }

            auto const fd_need_check_p{wasm_fd_storage.opens.begin() + wasm_fd_pos};
            fd_need_check_p->fd = fd;

            return {true, fd_need_check_p};
        }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check.fd == -1)
            {
                if(fd_need_check.close_pos < wasm_fd_storage.closes.size()) [[likely]]
                {
                    wasm_fd_storage.closes.erase(wasm_fd_storage.closes.cbegin() + fd_need_check.close_pos);
                }
                fd_need_check.close_pos = SIZE_MAX;
                fd_need_check.fd = fd;
            }
            else { fd_need_check.fd = fd; }

            return {true, __builtin_addressof(fd_need_check)};
        }
    }

    struct set_and_get_system_fd_t
    {
        bool success{};
        int sys_fd{-1};
    };

    inline set_and_get_system_fd_t set_and_get_system_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd, int fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};

        if(wasm_fd_pos >= ::uwvm::vm::wasi::wasi_fd_limit) [[unlikely]] { return {false, -1}; }

        int sys_fd{-1};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos)
        {
            auto const size_temp{wasm_fd_storage.opens.size()};
            wasm_fd_storage.opens.reserve(wasm_fd_pos + 1);
            if constexpr(::fast_io::freestanding::is_zero_default_constructible_v<wasm_fd>)
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                ::fast_io::freestanding::bytes_clear_n(
                    reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr),
                    static_cast<::std::size_t>(reinterpret_cast<::std::byte*>(end) - reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr)));
                wasm_fd_storage.opens.imp.curr_ptr = end;
            }
            else
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                for(; wasm_fd_storage.opens.imp.curr_ptr != end; ++wasm_fd_storage.opens.imp.curr_ptr)
                {
                    ::std::construct_at(wasm_fd_storage.opens.imp.curr_ptr);
                }
            }

            auto const new_size{wasm_fd_storage.opens.size()};
            for(::std::size_t i{size_temp}; i < new_size - 1; i++) { wasm_fd_storage.closes.push_back(i); }

            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            sys_fd = fd_need_check.fd;
            fd_need_check.fd = fd;

            return {true, sys_fd};
        }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check.fd == -1)
            {
                if(fd_need_check.close_pos < wasm_fd_storage.closes.size()) [[likely]]
                {
                    wasm_fd_storage.closes.erase(wasm_fd_storage.closes.cbegin() + fd_need_check.close_pos);
                }
                fd_need_check.close_pos = SIZE_MAX;
                sys_fd = fd_need_check.fd;
                fd_need_check.fd = fd;
            }
            else
            {
                sys_fd = fd_need_check.fd;
                fd_need_check.fd = fd;
            }

            return {true, sys_fd};
        }
    }

#ifdef UWVM_ENABLE_COPYED_WASMFD
    struct set_and_get_copy_wasm_fd_and_system_fd_t
    {
        bool success{};
        wasm_fd fd{};
        int sys_fd{-1};
    };

    inline set_and_get_copy_wasm_fd_and_system_fd_t
        set_and_get_copy_wasm_fd_and_system_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd, int fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};

        if(wasm_fd_pos >= ::uwvm::vm::wasi::wasi_fd_limit) [[unlikely]] { return {false, {}, -1}; }

        int sys_fd{-1};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos)
        {
            auto const size_temp{wasm_fd_storage.opens.size()};
            wasm_fd_storage.opens.reserve(wasm_fd_pos + 1);
            if constexpr(::fast_io::freestanding::is_zero_default_constructible_v<wasm_fd>)
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                ::fast_io::freestanding::bytes_clear_n(
                    reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr),
                    static_cast<::std::size_t>(reinterpret_cast<::std::byte*>(end) - reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr)));
                wasm_fd_storage.opens.imp.curr_ptr = end;
            }
            else
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                for(; wasm_fd_storage.opens.imp.curr_ptr != end; ++wasm_fd_storage.opens.imp.curr_ptr)
                {
                    ::std::construct_at(wasm_fd_storage.opens.imp.curr_ptr);
                }
            }

            auto const new_size{wasm_fd_storage.opens.size()};
            for(::std::size_t i{size_temp}; i < new_size - 1; i++) { wasm_fd_storage.closes.push_back(i); }

            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            sys_fd = fd_need_check.fd;
            fd_need_check.fd = fd;

            return {true, fd_need_check, sys_fd};
        }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check.fd == -1)
            {
                if(fd_need_check.close_pos < wasm_fd_storage.closes.size()) [[likely]]
                {
                    wasm_fd_storage.closes.erase(wasm_fd_storage.closes.cbegin() + fd_need_check.close_pos);
                }
                fd_need_check.close_pos = SIZE_MAX;
                sys_fd = fd_need_check.fd;
                fd_need_check.fd = fd;
            }
            else
            {
                sys_fd = fd_need_check.fd;
                fd_need_check.fd = fd;
            }

            return {true, fd_need_check, sys_fd};
        }
    }
#endif

    struct set_and_get_original_wasm_fd_p_and_system_fd_t
    {
        bool success{};
        wasm_fd* fd{};
        int sys_fd{-1};
    };

    inline set_and_get_original_wasm_fd_p_and_system_fd_t
        set_and_get_original_wasm_fd_p_and_system_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wfd, int fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wfd)};
        ::fast_io::io_lock_guard fds_lock{wasm_fd_storage.fds_mutex};

        if(wasm_fd_pos >= ::uwvm::vm::wasi::wasi_fd_limit) [[unlikely]] { return {false, {}, -1}; }

        int sys_fd{-1};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos)
        {
            auto const size_temp{wasm_fd_storage.opens.size()};
            wasm_fd_storage.opens.reserve(wasm_fd_pos + 1);
            if constexpr(::fast_io::freestanding::is_zero_default_constructible_v<wasm_fd>)
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                ::fast_io::freestanding::bytes_clear_n(
                    reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr),
                    static_cast<::std::size_t>(reinterpret_cast<::std::byte*>(end) - reinterpret_cast<::std::byte*>(wasm_fd_storage.opens.imp.curr_ptr)));
                wasm_fd_storage.opens.imp.curr_ptr = end;
            }
            else
            {
                auto const end{wasm_fd_storage.opens.imp.curr_ptr + (wasm_fd_pos + 1u - size_temp)};
                for(; wasm_fd_storage.opens.imp.curr_ptr != end; ++wasm_fd_storage.opens.imp.curr_ptr)
                {
                    ::std::construct_at(wasm_fd_storage.opens.imp.curr_ptr);
                }
            }

            auto const new_size{wasm_fd_storage.opens.size()};
            for(::std::size_t i{size_temp}; i < new_size - 1; i++) { wasm_fd_storage.closes.push_back(i); }

            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            sys_fd = fd_need_check.fd;
            fd_need_check.fd = fd;

            return {true, __builtin_addressof(fd_need_check), sys_fd};
        }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check.fd == -1)
            {
                if(fd_need_check.close_pos < wasm_fd_storage.closes.size()) [[likely]]
                {
                    wasm_fd_storage.closes.erase(wasm_fd_storage.closes.cbegin() + fd_need_check.close_pos);
                }
                fd_need_check.close_pos = SIZE_MAX;
                sys_fd = fd_need_check.fd;
                fd_need_check.fd = fd;
            }
            else
            {
                sys_fd = fd_need_check.fd;
                fd_need_check.fd = fd;
            }

            return {true, __builtin_addressof(fd_need_check), sys_fd};
        }
    }

    // storage
    inline wasm_fd_storage_t wasm_fd_storages{};

}  // namespace uwvm::vm::wasi

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::wasi::wasm_fd_storage_t>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::wasi::wasm_fd_storage_t>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding

