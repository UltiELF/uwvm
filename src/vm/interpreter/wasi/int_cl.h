#pragma once
#include <fast_io.h>
#include <fast_io_dsal/string_view.h>
#include <map>  // !
#include "../import.h"

namespace uwvm::vm::interpreter::wasi
{
    // interpreter compatibility layer

    void int_args_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_args_sizes_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_environ_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_environ_sizes_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_clock_res_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_clock_time_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_advise(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_allocate(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_close(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_datasync(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_fdstat_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_fdstat_set_flags(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_fdstat_set_rights(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_filestat_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_filestat_set_size(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_filestat_set_times(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_pread(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_prestat_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_prestat_dir_name(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_pwrite(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_read(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_readdir(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_renumber(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_seek(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_sync(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_tell(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_fd_write(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_create_directory(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_filestat_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_filestat_set_times(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_link(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_open(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_readlink(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_remove_directory(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_rename(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_symlink(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_path_unlink_file(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_poll_oneoff(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_proc_exit(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_sched_yield(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_random_get(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_sock_accept(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_sock_recv(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_sock_send(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    void int_sock_shutdown(::uwvm::vm::interpreter::stack_t const* begin, ::uwvm::vm::interpreter::stack_machine& sm) noexcept {}

    ::std::map<::fast_io::u8string_view, ::uwvm::vm::interpreter::int_import_func_p> wasi_map{

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
}  // namespace uwvm::vm::interpreter::wasi
