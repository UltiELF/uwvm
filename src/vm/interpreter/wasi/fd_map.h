#pragma once

#include <fast_io.h>
#include <fast_io_dsal/vector.h>

namespace uwvm::vm::interpreter::wasi
{
    struct wasm_fd_storage_t
    {
        ::fast_io::vector<int> opens{};
        ::fast_io::vector<::std::size_t> closes{};
    };

    inline constexpr void init(wasm_fd_storage_t& wasm_fd_storage) noexcept
    {
#ifdef __MSDOS__
        wasm_fd_storage.opens.reserve(1024);
        wasm_fd_storage.closes.reserve(1024);
#else
        wasm_fd_storage.opens.reserve(32768);
        wasm_fd_storage.closes.reserve(32768);
#endif
    }

    inline constexpr int get_fd(wasm_fd_storage_t const& wasm_fd_storage, ::std::int_least32_t wasm_fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wasm_fd)};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]] { return -1; }
        else { return wasm_fd_storage.opens.index_unchecked(wasm_fd_pos); }
    }

    inline constexpr ::std::int_least32_t create_wasm_fd(wasm_fd_storage_t& wasm_fd_storage, int fd) noexcept
    {
        if(wasm_fd_storage.closes.empty())
        {
            auto const pos_p{__builtin_addressof(wasm_fd_storage.opens.emplace_back(fd))};
            return static_cast<::std::int_least32_t>(pos_p - wasm_fd_storage.opens.begin());
        }
        else
        {
            auto const pos{wasm_fd_storage.closes.back_unchecked()};
            wasm_fd_storage.closes.pop_back_unchecked();
            wasm_fd_storage.opens.index_unchecked(pos) = fd;
            return static_cast<::std::int_least32_t>(pos);
        }
    }

    inline constexpr bool delete_wasm_fd(wasm_fd_storage_t& wasm_fd_storage, ::std::int_least32_t wasm_fd) noexcept
    {
        auto const wasm_fd_pos{static_cast<::std::size_t>(wasm_fd)};
        if(wasm_fd_storage.opens.size() <= wasm_fd_pos) [[unlikely]] { return false; }
        else
        {
            auto& fd_need_check{wasm_fd_storage.opens.index_unchecked(wasm_fd_pos)};
            if(fd_need_check == -1) [[unlikely]] { return false; }

            fd_need_check = -1;
            wasm_fd_storage.closes.push_back(wasm_fd_pos);
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

