#pragma once
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
#include "../import.h"
#include "../../wasi/wasi.h"
#include "../hash_table.h"

namespace uwvm::vm::interpreter::wasi
{
    // interpreter compatibility layer

    void int_args_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::args_get(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_args_sizes_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::args_sizes_get(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_environ_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::environ_get(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_environ_sizes_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::environ_sizes_get(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_clock_res_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::clock_res_get(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_clock_time_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::clock_time_get(begin[0].i32, begin[1].i64, begin[2].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_advise(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_advise(begin[0].i32, begin[1].i64, begin[2].i64, begin[3].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_allocate(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_allocate(begin[0].i32, begin[1].i64, begin[2].i64)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_close(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_close(begin[0].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_datasync(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_datasync(begin[0].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_fdstat_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_fdstat_get(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_fdstat_set_flags(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_fdstat_set_flags(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_fdstat_set_rights(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_fdstat_set_rights(begin[0].i32, begin[1].i64, begin[2].i64)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_filestat_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_filestat_get(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_filestat_set_size(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_filestat_set_size(begin[0].i32, begin[1].i64)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_filestat_set_times(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_filestat_set_times(begin[0].i32, begin[1].i64, begin[2].i64, begin[3].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_pread(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_pread(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i64, begin[4].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_prestat_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_prestat_get(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_prestat_dir_name(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_prestat_dir_name(begin[0].i32, begin[1].i32, begin[2].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_pwrite(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_pwrite(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i64, begin[4].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_read(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_read(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_readdir(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_readdir(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i64, begin[4].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_renumber(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_renumber(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_seek(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_seek(begin[0].i32, begin[1].i64, begin[2].i32, begin[3].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_sync(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_sync(begin[0].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_tell(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_tell(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_fd_write(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::fd_write(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_create_directory(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::path_create_directory(begin[0].i32, begin[1].i32, begin[2].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_filestat_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::path_filestat_get(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32, begin[4].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_filestat_set_times(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::path_filestat_set_times(begin[0].i32,
                                                                                        begin[1].i32,
                                                                                        begin[2].i32,
                                                                                        begin[3].i32,
                                                                                        begin[4].i64,
                                                                                        begin[5].i64,
                                                                                        begin[6].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_link(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{
            ::uwvm::vm::wasi::path_link(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32, begin[4].i32, begin[5].i32, begin[6].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_open(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::path_open(begin[0].i32,
                                                                          begin[1].i32,
                                                                          begin[2].i32,
                                                                          begin[3].i32,
                                                                          begin[4].i32,
                                                                          begin[5].i64,
                                                                          begin[6].i64,
                                                                          begin[7].i32,
                                                                          begin[8].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_readlink(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{
            ::uwvm::vm::wasi::path_readlink(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32, begin[4].i32, begin[5].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_remove_directory(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::path_remove_directory(begin[0].i32, begin[1].i32, begin[2].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_rename(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{
            ::uwvm::vm::wasi::path_rename(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32, begin[4].i32, begin[5].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_symlink(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::path_symlink(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32, begin[4].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_path_unlink_file(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::path_unlink_file(begin[0].i32, begin[1].i32, begin[2].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_poll_oneoff(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::poll_oneoff(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_proc_exit(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::uwvm::vm::wasi::proc_exit(begin[0].i32);
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
    }

    void int_sched_yield(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::sched_yield()};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_random_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::random_get(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_sock_accept(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::sock_accept(begin[0].i32, begin[1].i32, begin[2].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_sock_recv(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::sock_recv(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32, begin[4].i32, begin[5].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_sock_send(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::sock_send(begin[0].i32, begin[1].i32, begin[2].i32, begin[3].i32, begin[4].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    void int_sock_shutdown(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept
    {
        ::std::int_least32_t ret{::uwvm::vm::wasi::sock_shutdown(begin[0].i32, begin[1].i32)};
        sm.stack.get_container().imp.curr_ptr = const_cast<::uwvm::vm::interpreter::stack_t*>(begin);
        sm.stack.push_unchecked(::uwvm::vm::interpreter::stack_t{.i32{ret}, .vt{::uwvm::wasm::value_type::i32}});
    }

    inline constexpr ::uwvm::vm::interpreter::import_pair wasi_int_pair[]{

        {::fast_io::u8string_view{u8"args_get"},                __builtin_addressof(int_args_get)               },

        {::fast_io::u8string_view{u8"args_sizes_get"},          __builtin_addressof(int_args_sizes_get)         },

        {::fast_io::u8string_view{u8"environ_get"},             __builtin_addressof(int_environ_get)            },

        {::fast_io::u8string_view{u8"environ_sizes_get"},       __builtin_addressof(int_environ_sizes_get)      },

        {::fast_io::u8string_view{u8"clock_res_get"},           __builtin_addressof(int_clock_res_get)          },

        {::fast_io::u8string_view{u8"clock_time_get"},          __builtin_addressof(int_clock_time_get)         },

        {::fast_io::u8string_view{u8"fd_advise"},               __builtin_addressof(int_fd_advise)              },

        {::fast_io::u8string_view{u8"fd_allocate"},             __builtin_addressof(int_fd_allocate)            },

        {::fast_io::u8string_view{u8"fd_close"},                __builtin_addressof(int_fd_close)               },

        {::fast_io::u8string_view{u8"fd_datasync"},             __builtin_addressof(int_fd_datasync)            },

        {::fast_io::u8string_view{u8"fd_fdstat_get"},           __builtin_addressof(int_fd_fdstat_get)          },

        {::fast_io::u8string_view{u8"fd_fdstat_set_flags"},     __builtin_addressof(int_fd_fdstat_set_flags)    },

        {::fast_io::u8string_view{u8"fd_fdstat_set_rights"},    __builtin_addressof(int_fd_fdstat_set_rights)   },

        {::fast_io::u8string_view{u8"fd_filestat_get"},         __builtin_addressof(int_fd_filestat_get)        },

        {::fast_io::u8string_view{u8"fd_filestat_set_size"},    __builtin_addressof(int_fd_filestat_set_size)   },

        {::fast_io::u8string_view{u8"fd_filestat_set_times"},   __builtin_addressof(int_fd_filestat_set_times)  },

        {::fast_io::u8string_view{u8"fd_pread"},                __builtin_addressof(int_fd_pread)               },

        {::fast_io::u8string_view{u8"fd_prestat_get"},          __builtin_addressof(int_fd_prestat_get)         },

        {::fast_io::u8string_view{u8"fd_prestat_dir_name"},     __builtin_addressof(int_fd_prestat_dir_name)    },

        {::fast_io::u8string_view{u8"fd_pwrite"},               __builtin_addressof(int_fd_pwrite)              },

        {::fast_io::u8string_view{u8"fd_read"},                 __builtin_addressof(int_fd_read)                },

        {::fast_io::u8string_view{u8"fd_readdir"},              __builtin_addressof(int_fd_readdir)             },

        {::fast_io::u8string_view{u8"fd_renumber"},             __builtin_addressof(int_fd_renumber)            },

        {::fast_io::u8string_view{u8"fd_seek"},                 __builtin_addressof(int_fd_seek)                },

        {::fast_io::u8string_view{u8"fd_sync"},                 __builtin_addressof(int_fd_sync)                },

        {::fast_io::u8string_view{u8"fd_tell"},                 __builtin_addressof(int_fd_tell)                },

        {::fast_io::u8string_view{u8"fd_write"},                __builtin_addressof(int_fd_write)               },

        {::fast_io::u8string_view{u8"path_create_directory"},   __builtin_addressof(int_path_create_directory)  },

        {::fast_io::u8string_view{u8"path_filestat_get"},       __builtin_addressof(int_path_filestat_get)      },

        {::fast_io::u8string_view{u8"path_filestat_set_times"}, __builtin_addressof(int_path_filestat_set_times)},

        {::fast_io::u8string_view{u8"path_link"},               __builtin_addressof(int_path_link)              },

        {::fast_io::u8string_view{u8"path_open"},               __builtin_addressof(int_path_open)              },

        {::fast_io::u8string_view{u8"path_readlink"},           __builtin_addressof(int_path_readlink)          },

        {::fast_io::u8string_view{u8"path_remove_directory"},   __builtin_addressof(int_path_remove_directory)  },

        {::fast_io::u8string_view{u8"path_rename"},             __builtin_addressof(int_path_rename)            },

        {::fast_io::u8string_view{u8"path_symlink"},            __builtin_addressof(int_path_symlink)           },

        {::fast_io::u8string_view{u8"path_unlink_file"},        __builtin_addressof(int_path_unlink_file)       },

        {::fast_io::u8string_view{u8"poll_oneoff"},             __builtin_addressof(int_poll_oneoff)            },

        {::fast_io::u8string_view{u8"proc_exit"},               __builtin_addressof(int_proc_exit)              },

        {::fast_io::u8string_view{u8"sched_yield"},             __builtin_addressof(int_sched_yield)            },

        {::fast_io::u8string_view{u8"random_get"},              __builtin_addressof(int_random_get)             },

        {::fast_io::u8string_view{u8"sock_accept"},             __builtin_addressof(int_sock_accept)            },

        {::fast_io::u8string_view{u8"sock_recv"},               __builtin_addressof(int_sock_recv)              },

        {::fast_io::u8string_view{u8"sock_send"},               __builtin_addressof(int_sock_send)              },

        {::fast_io::u8string_view{u8"sock_shutdown"},           __builtin_addressof(int_sock_shutdown)          },
    };

    inline constexpr auto basic_wasi_array{::uwvm::vm::interpreter::check_import(wasi_int_pair)};

    inline constexpr auto wasi_hash_table_size{::uwvm::vm::interpreter::generate_hash_table_size(basic_wasi_array)};
    inline constexpr auto wasi_hash_table{
        ::uwvm::vm::interpreter::generate_hash_table<wasi_hash_table_size.size, wasi_hash_table_size.extra_size>(basic_wasi_array)};

    inline constexpr auto sizeof_wasi_hash_table{sizeof(wasi_hash_table)};

}  // namespace uwvm::vm::wasi
