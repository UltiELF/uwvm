#pragma once
#include <cstdint>
#include <cstddef>

namespace uwvm::vm::wasi
{
    enum class wasi_intptr_t : ::std::int_least32_t
    {

    };

    enum class wasi_uintptr_t : ::std::uint_least32_t
    {

    };

    enum class wasi_size_t : ::std::uint_least32_t
    {

    };

    enum class wasi_void_ptr_t : ::std::uint_least32_t
    {

    };

#if 0
    static_assert(alignof(::std::int_least8_t) == 1, "non-wasi data layout");
    static_assert(alignof(::std::uint_least8_t) == 1, "non-wasi data layout");
    static_assert(alignof(::std::int_least16_t) == 2, "non-wasi data layout");
    static_assert(alignof(::std::uint_least16_t) == 2, "non-wasi data layout");
    static_assert(alignof(::std::int_least32_t) == 4, "non-wasi data layout");
    static_assert(alignof(::std::uint_least32_t) == 4, "non-wasi data layout");
    static_assert(alignof(::std::int_least64_t) == 8, "non-wasi data layout");
    static_assert(alignof(::std::uint_least64_t) == 8, "non-wasi data layout");
#endif

    enum class advice_t : ::std::uint_least8_t
    {
        advice_normal,
        advice_sequential,
        advice_random,
        advice_willneed,
        advice_dontneed,
        advice_noreuse,

    };

    enum class clockid_t : ::std::uint_least32_t
    {
        clock_realtime,
        clock_monotonic,
        clock_process_cputime_id,
        clock_thread_cputime_id,

    };

    enum class device_t : ::std::uint_least64_t
    {

    };

    enum class dirnamlen_t : ::std::uint_least32_t
    {

    };

    enum class dircookie_t : ::std::uint_least64_t
    {
        dircookie_start
    };

    enum class errno_t : ::std::uint_least16_t
    {
        esuccess,
        e2big,
        eacces,
        eaddrinuse,
        eaddrnotavail,
        eafnosupport,
        eagain,
        ealready,
        ebadf,
        ebadmsg,
        ebusy,
        ecanceled,
        echild,
        econnaborted,
        econnrefused,
        econnreset,
        edeadlk,
        edestaddrreq,
        edom,
        edquot,
        eexist,
        efault,
        efbig,
        ehostunreach,
        eidrm,
        eilseq,
        einprogress,
        eintr,
        einval,
        eio,
        eisconn,
        eisdir,
        eloop,
        emfile,
        emlink,
        emsgsize,
        emultihop,
        enametoolong,
        enetdown,
        enetreset,
        enetunreach,
        enfile,
        enobufs,
        enodev,
        enoent,
        enoexec,
        enolck,
        enolink,
        enomem,
        enomsg,
        enoprotoopt,
        enospc,
        enosys,
        enotconn,
        enotdir,
        enotempty,
        enotrecoverable,
        enotsock,
        enotsup,
        enotty,
        enxio,
        eoverflow,
        eownerdead,
        eperm,
        epipe,
        eproto,
        eprotonosupport,
        eprototype,
        erange,
        erofs,
        espipe,
        esrch,
        estale,
        etimedout,
        etxtbsy,
        exdev,
        enotcapable,
    };

    enum class eventrwflags_t : ::std::uint_least16_t
    {
        event_fd_readwrite_hangup = 0x0001
    };

    enum class eventtype_t : ::std::uint_least8_t
    {
        eventtype_clock,
        eventtype_fd_read,
        eventtype_fd_write,
    };

    enum class exitcode_t : ::std::uint_least32_t
    {

    };

    enum class fd_t : ::std::uint_least32_t
    {

    };

    enum class fdflags_t : ::std::uint_least16_t
    {
        fdflag_append = 0x0001,
        fdflag_dsync = 0x0002,
        fdflag_nonblock = 0x0004,
        fdflag_rsync = 0x0008,
        fdflag_sync = 0x0010,
    };

    enum class filedelta_t : ::std::int_least64_t
    {

    };

    enum class filesize_t : ::std::uint_least64_t
    {

    };

    enum class filetype_t : ::std::uint_least8_t
    {
        filetype_unknown,
        filetype_block_device,
        filetype_character_device,
        filetype_directory,
        filetype_regular_file,
        filetype_socket_dgram,
        filetype_socket_stream,
        filetype_symbolic_link,
    };

    enum class fstflags_t : ::std::uint_least16_t
    {
        filestat_set_atim = 0x0001,
        filestat_set_atim_now = 0x0002,
        filestat_set_mtim = 0x0004,
        filestat_set_mtim_now = 0x0008,
    };

    enum class inode_t : ::std::uint_least64_t
    {

    };

    enum class linkcount_t : ::std::uint_least64_t
    {

    };

    enum class lookupflags_t : ::std::uint_least32_t
    {
        lookup_symlink_follow = 0x00000001
    };

    enum class oflags_t : ::std::uint_least16_t
    {
        o_creat = 0x0001,
        o_directory = 0x0002,
        o_excl = 0x0004,
        o_trunc = 0x0008,
    };

    enum class riflags_t : ::std::uint_least16_t
    {
        sock_recv_peek = 0x0001,
        sock_recv_waitall = 0x0002,
    };

    enum class rights_t : ::std::uint_least64_t
    {
        right_fd_datasync = 0x0000000000000001,
        right_fd_read = 0x0000000000000002,
        right_fd_seek = 0x0000000000000004,
        right_fd_fdstat_set_flags = 0x0000000000000008,
        right_fd_sync = 0x0000000000000010,
        right_fd_tell = 0x0000000000000020,
        right_fd_write = 0x0000000000000040,
        right_fd_advise = 0x0000000000000080,
        right_fd_allocate = 0x0000000000000100,
        right_path_create_directory = 0x0000000000000200,
        right_path_create_file = 0x0000000000000400,
        right_path_link_source = 0x0000000000000800,
        right_path_link_target = 0x0000000000001000,
        right_path_open = 0x0000000000002000,
        right_fd_readdir = 0x0000000000004000,
        right_path_readlink = 0x0000000000008000,
        right_path_rename_source = 0x0000000000010000,
        right_path_rename_target = 0x0000000000020000,
        right_path_filestat_get = 0x0000000000040000,
        right_path_filestat_set_size = 0x0000000000080000,
        right_path_filestat_set_times = 0x0000000000100000,
        right_fd_filestat_get = 0x0000000000200000,
        right_fd_filestat_set_size = 0x0000000000400000,
        right_fd_filestat_set_times = 0x0000000000800000,
        right_path_symlink = 0x0000000001000000,
        right_path_remove_directory = 0x0000000002000000,
        right_path_unlink_file = 0x0000000004000000,
        right_poll_fd_readwrite = 0x0000000008000000,
        right_sock_shutdown = 0x0000000010000000,
    };

    enum class roflags_t : ::std::uint_least16_t
    {
        sock_recv_data_truncated = 0x0001,
    };

    enum class sdflags_t : ::std::uint_least8_t
    {
        shut_rd = 0x01,
        shut_wr = 0x02,
    };

    enum class siflags_t : ::std::uint_least16_t
    {

    };

    enum class signal_t : ::std::uint_least8_t
    {
        sighup = 1,
        sigint,
        sigquit,
        sigill,
        sigtrap,
        sigabrt,
        sigbus,
        sigfpe,
        sigkill,
        sigusr1,
        sigsegv,
        sigusr2,
        sigpipe,
        sigalrm,
        sigterm,
        sigchld,
        sigcont,
        sigstop,
        sigtstp,
        sigttin,
        sigttou,
        sigurg,
        sigxcpu,
        sigxfsz,
        sigvtalrm,
        sigprof,
        sigwinch,
        sigpoll,
        sigpwr,
        sigsys,

    };

    enum class subclockflags_t : ::std::uint_least16_t
    {
        subscription_clock_abstime = 0x0001
    };

    enum class timestamp_t : ::std::uint_least64_t
    {

    };

    enum class userdata_t : ::std::uint_least64_t
    {

    };

    enum class whence_t : ::std::uint_least8_t
    {
        whence_set,
        whence_cur,
        whence_end,

    };

    enum class preopentype_t : ::std::uint_least8_t
    {
        preopentype_dir
    };

#if 0
    struct dirent_t
    {
        dircookie_t d_next{};
        inode_t d_ino{};
        dirnamlen_t d_namlen{};
        filetype_t d_type{};
    };

    static_assert(__builtin_offsetof(dirent_t, d_next) == 0, "non-wasi data layout");
    static_assert(__builtin_offsetof(dirent_t, d_ino) == 8, "non-wasi data layout");
    static_assert(__builtin_offsetof(dirent_t, d_namlen) == 16, "non-wasi data layout");
    static_assert(__builtin_offsetof(dirent_t, d_type) == 20, "non-wasi data layout");
    static_assert(sizeof(dirent_t) == 24, "non-wasi data layout");
    static_assert(alignof(dirent_t) == 8, "non-wasi data layout");

    struct event_t
    {
        userdata_t userdata{};
        errno_t error{};
        eventtype_t type{};

        union event_u
        {
            struct event_u_fd_readwrite_t
            {
                filesize_t nbytes{};
                eventrwflags_t flags{};
            } fd_readwrite;
        } u;
    };

    static_assert(__builtin_offsetof(event_t, userdata) == 0, "non-wasi data layout");
    static_assert(__builtin_offsetof(event_t, error) == 8, "non-wasi data layout");
    static_assert(__builtin_offsetof(event_t, type) == 10, "non-wasi data layout");
    static_assert(__builtin_offsetof(event_t, u.fd_readwrite.nbytes) == 16, "non-wasi data layout");
    static_assert(__builtin_offsetof(event_t, u.fd_readwrite.flags) == 24, "non-wasi data layout");
    static_assert(sizeof(event_t) == 32, "non-wasi data layout");
    static_assert(alignof(event_t) == 8, "non-wasi data layout");

    struct prestat_t
    {
        preopentype_t pr_type{};

        union prestat_u
        {
            struct prestat_u_dir_t
            {
                wasi_size_t pr_name_len{};
            } dir;
        } u;
    };

    static_assert(__builtin_offsetof(prestat_t, pr_type) == 0, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 4 || __builtin_offsetof(prestat_t, u.dir.pr_name_len) == 4, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 8 || __builtin_offsetof(prestat_t, u.dir.pr_name_len) == 8, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 4 || sizeof(prestat_t) == 8, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 8 || sizeof(prestat_t) == 16, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 4 || alignof(prestat_t) == 4, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 8 || alignof(prestat_t) == 8, "non-wasi data layout");

    struct fdstat_t
    {
        filetype_t fs_filetype{};
        fdflags_t fs_flags{};
        rights_t fs_rights_base{};
        rights_t fs_rights_inheriting{};
    };

    static_assert(__builtin_offsetof(fdstat_t, fs_filetype) == 0, "non-wasi data layout");
    static_assert(__builtin_offsetof(fdstat_t, fs_flags) == 2, "non-wasi data layout");
    static_assert(__builtin_offsetof(fdstat_t, fs_rights_base) == 8, "non-wasi data layout");
    static_assert(__builtin_offsetof(fdstat_t, fs_rights_inheriting) == 16, "non-wasi data layout");
    static_assert(sizeof(fdstat_t) == 24, "non-wasi data layout");
    static_assert(alignof(fdstat_t) == 8, "non-wasi data layout");

    struct filestat_t
    {
        device_t st_dev{};
        inode_t st_ino{};
        filetype_t st_filetype{};
        linkcount_t st_nlink{};
        filesize_t st_size{};
        timestamp_t st_atim{};
        timestamp_t st_mtim{};
        timestamp_t st_ctim{};
    };

    static_assert(__builtin_offsetof(filestat_t, st_dev) == 0, "non-wasi data layout");
    static_assert(__builtin_offsetof(filestat_t, st_ino) == 8, "non-wasi data layout");
    static_assert(__builtin_offsetof(filestat_t, st_filetype) == 16, "non-wasi data layout");
    static_assert(__builtin_offsetof(filestat_t, st_nlink) == 24, "non-wasi data layout");
    static_assert(__builtin_offsetof(filestat_t, st_size) == 32, "non-wasi data layout");
    static_assert(__builtin_offsetof(filestat_t, st_atim) == 40, "non-wasi data layout");
    static_assert(__builtin_offsetof(filestat_t, st_mtim) == 48, "non-wasi data layout");
    static_assert(__builtin_offsetof(filestat_t, st_ctim) == 56, "non-wasi data layout");
    static_assert(sizeof(filestat_t) == 64, "non-wasi data layout");
    static_assert(alignof(filestat_t) == 8, "non-wasi data layout");

    struct ciovec_t
    {
        wasi_void_ptr_t buf{};
        wasi_size_t buf_len{};
    };

    static_assert(__builtin_offsetof(ciovec_t, buf) == 0, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 4 || __builtin_offsetof(ciovec_t, buf_len) == 4, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 8 || __builtin_offsetof(ciovec_t, buf_len) == 8, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 4 || sizeof(ciovec_t) == 8, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 8 || sizeof(ciovec_t) == 16, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 4 || alignof(ciovec_t) == 4, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 8 || alignof(ciovec_t) == 8, "non-wasi data layout");

    struct iovec_t
    {
        wasi_void_ptr_t buf{};
        wasi_size_t buf_len{};
    };

    static_assert(__builtin_offsetof(iovec_t, buf) == 0, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 4 || __builtin_offsetof(iovec_t, buf_len) == 4, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 8 || __builtin_offsetof(iovec_t, buf_len) == 8, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 4 || sizeof(iovec_t) == 8, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 8 || sizeof(iovec_t) == 16, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 4 || alignof(iovec_t) == 4, "non-wasi data layout");
    static_assert(sizeof(wasi_void_ptr_t) != 8 || alignof(iovec_t) == 8, "non-wasi data layout");

    struct subscription_t
    {
        userdata_t userdata{};
        eventtype_t type{};

        union subscription_u
        {
            struct subscription_u_clock_t
            {
                clockid_t clock_id{};
                timestamp_t timeout{};
                timestamp_t precision{};
                subclockflags_t flags{};
            } clock;

            struct subscription_u_fd_readwrite_t
            {
                fd_t fd{};
            } fd_readwrite;
        } u;
    };

    static_assert(__builtin_offsetof(subscription_t, userdata) == 0, "non-wasi data layout");
    static_assert(__builtin_offsetof(subscription_t, type) == 8, "non-wasi data layout");
    static_assert(__builtin_offsetof(subscription_t, u.clock.clock_id) == 16, "non-wasi data layout");
    static_assert(__builtin_offsetof(subscription_t, u.clock.timeout) == 24, "non-wasi data layout");
    static_assert(__builtin_offsetof(subscription_t, u.clock.precision) == 32, "non-wasi data layout");
    static_assert(__builtin_offsetof(subscription_t, u.clock.flags) == 40, "non-wasi data layout");
    static_assert(__builtin_offsetof(subscription_t, u.fd_readwrite.fd) == 16, "non-wasi data layout");
    static_assert(sizeof(subscription_t) == 48, "non-wasi data layout");
    static_assert(alignof(subscription_t) == 8, "non-wasi data layout");
#endif
}  // namespace uwvm::vm::interpreter::wasi
