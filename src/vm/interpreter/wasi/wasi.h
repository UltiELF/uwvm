#pragma once
#include <fast_io.h>
#include "abi.h"

namespace uwvm::vm::interpreter::wasi
{
    ::std::int_least32_t args_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t args_sizes_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t environ_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t environ_sizes_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t clock_res_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t clock_time_get(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least32_t arg2) noexcept { return {}; }

    ::std::int_least32_t fd_advise(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least64_t arg2, ::std::int_least32_t arg3) noexcept
    {
        return {};
    }

    ::std::int_least32_t fd_allocate(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least64_t arg2) noexcept { return {}; }

    ::std::int_least32_t fd_close(::std::int_least32_t arg0) noexcept { return {}; }

    ::std::int_least32_t fd_datasync(::std::int_least32_t arg0) noexcept { return {}; }

    ::std::int_least32_t fd_fdstat_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t fd_fdstat_set_flags(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t fd_fdstat_set_rights(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least64_t arg2) noexcept { return {}; }

    ::std::int_least32_t fd_filestat_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t fd_filestat_set_size(::std::int_least32_t arg0, ::std::int_least64_t arg1) noexcept { return {}; }

    ::std::int_least32_t
        fd_filestat_set_times(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least64_t arg2, ::std::int_least32_t arg3) noexcept
    {
        return {};
    }

    ::std::int_least32_t
        fd_pread(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2, ::std::int_least64_t arg3, ::std::int_least32_t arg4) noexcept
    {
        return {};
    }

    ::std::int_least32_t fd_prestat_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t fd_prestat_dir_name(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2) noexcept { return {}; }

    ::std::int_least32_t fd_pwrite(::std::int_least32_t arg0,
                                   ::std::int_least32_t arg1,
                                   ::std::int_least32_t arg2,
                                   ::std::int_least64_t arg3,
                                   ::std::int_least32_t arg4) noexcept
    {
        return {};
    }

    ::std::int_least32_t fd_read(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2, ::std::int_least32_t arg3) noexcept
    {
        return {};
    }

    ::std::int_least32_t fd_readdir(::std::int_least32_t arg0,
                                    ::std::int_least32_t arg1,
                                    ::std::int_least32_t arg2,
                                    ::std::int_least64_t arg3,
                                    ::std::int_least32_t arg4) noexcept
    {
        return {};
    }

    ::std::int_least32_t fd_renumber(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t fd_seek(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least32_t arg2, ::std::int_least32_t arg3) noexcept
    {
        return {};
    }

    ::std::int_least32_t fd_sync(::std::int_least32_t arg0) noexcept { return {}; }

    ::std::int_least32_t fd_tell(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t fd_write(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2, ::std::int_least32_t arg3) noexcept
    {
        // No need to check the memory of wasm
        ::fast_io::posix_io_observer pfd{static_cast<int>(arg0)};
        ::uwvm::vm::interpreter::wasi::ciovec_t cvt{};
        ::std::byte* memory_begin{::uwvm::vm::interpreter::memories.front().memory_begin};

        ::std::byte const* cvt_begin{memory_begin + static_cast<::std::size_t>(arg1)};
        ::fast_io::freestanding::my_memcpy(__builtin_addressof(cvt), cvt_begin, sizeof(cvt));

        auto const cvt_buf_off{static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(cvt.buf))};
        auto const cvt_buf_begin{memory_begin + cvt_buf_off};
        if(static_cast<::std::uint_least32_t>(cvt.buf_len) == 0) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }
        auto const cvt_buf_write_end{::fast_io::operations::write_some_bytes(pfd, cvt_buf_begin, cvt_buf_begin + static_cast<::std::size_t>(cvt.buf_len))};

        auto const cvt_buf_write_sz{static_cast<::std::size_t>(cvt_buf_write_end - cvt_buf_begin)};
        if(cvt_buf_write_sz != static_cast<::std::size_t>(cvt.buf_len)) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::std::byte* number_begin{memory_begin + static_cast<::std::size_t>(arg3)};
        ::std::uint_least32_t le32{::fast_io::little_endian(static_cast<::std::uint_least32_t>(cvt_buf_write_sz))};
        ::fast_io::freestanding::my_memcpy(number_begin, __builtin_addressof(le32), sizeof(le32));

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t path_create_directory(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2) noexcept { return {}; }

    ::std::int_least32_t path_filestat_get(::std::int_least32_t arg0,
                                           ::std::int_least32_t arg1,
                                           ::std::int_least32_t arg2,
                                           ::std::int_least32_t arg3,
                                           ::std::int_least32_t arg4) noexcept
    {
        return {};
    }

    ::std::int_least32_t path_filestat_set_times(::std::int_least32_t arg0,
                                                 ::std::int_least32_t arg1,
                                                 ::std::int_least32_t arg2,
                                                 ::std::int_least32_t arg3,
                                                 ::std::int_least64_t arg4,
                                                 ::std::int_least64_t arg5,
                                                 ::std::int_least32_t arg6) noexcept
    {
        return {};
    }

    ::std::int_least32_t path_link(::std::int_least32_t arg0,
                                   ::std::int_least32_t arg1,
                                   ::std::int_least32_t arg2,
                                   ::std::int_least32_t arg3,
                                   ::std::int_least32_t arg4,
                                   ::std::int_least32_t arg5,
                                   ::std::int_least32_t arg6) noexcept
    {
        return {};
    }

    ::std::int_least32_t path_open(::std::int_least32_t arg0,
                                   ::std::int_least32_t arg1,
                                   ::std::int_least32_t arg2,
                                   ::std::int_least32_t arg3,
                                   ::std::int_least32_t arg4,
                                   ::std::int_least64_t arg5,
                                   ::std::int_least64_t arg6,
                                   ::std::int_least32_t arg7,
                                   ::std::int_least32_t arg8) noexcept
    {
        return {};
    }

    ::std::int_least32_t path_readlink(::std::int_least32_t arg0,
                                       ::std::int_least32_t arg1,
                                       ::std::int_least32_t arg2,
                                       ::std::int_least32_t arg3,
                                       ::std::int_least32_t arg4,
                                       ::std::int_least32_t arg5) noexcept
    {
        return {};
    }

    ::std::int_least32_t path_remove_directory(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2) noexcept { return {}; }

    ::std::int_least32_t path_rename(::std::int_least32_t arg0,
                                     ::std::int_least32_t arg1,
                                     ::std::int_least32_t arg2,
                                     ::std::int_least32_t arg3,
                                     ::std::int_least32_t arg4,
                                     ::std::int_least32_t arg5) noexcept
    {
        return {};
    }

    ::std::int_least32_t path_symlink(::std::int_least32_t arg0,
                                      ::std::int_least32_t arg1,
                                      ::std::int_least32_t arg2,
                                      ::std::int_least32_t arg3,
                                      ::std::int_least32_t arg4) noexcept
    {
        return {};
    }

    ::std::int_least32_t path_unlink_file(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2) noexcept { return {}; }

    ::std::int_least32_t poll_oneoff(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2, ::std::int_least32_t arg3) noexcept
    {
        return {};
    }

    void proc_exit(::std::int_least32_t arg0) noexcept { return; }

    ::std::int_least32_t sched_yield() noexcept { return {}; }

    ::std::int_least32_t random_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t sock_accept(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2) noexcept { return {}; }

    ::std::int_least32_t sock_recv(::std::int_least32_t arg0,
                                   ::std::int_least32_t arg1,
                                   ::std::int_least32_t arg2,
                                   ::std::int_least32_t arg3,
                                   ::std::int_least32_t arg4,
                                   ::std::int_least32_t arg5) noexcept
    {
        return {};
    }

    ::std::int_least32_t sock_send(::std::int_least32_t arg0,
                                   ::std::int_least32_t arg1,
                                   ::std::int_least32_t arg2,
                                   ::std::int_least32_t arg3,
                                   ::std::int_least32_t arg4) noexcept
    {
        return {};
    }

    ::std::int_least32_t sock_shutdown(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

}  // namespace uwvm::vm::interpreter::wasi
