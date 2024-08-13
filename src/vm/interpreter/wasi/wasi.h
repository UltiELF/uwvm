#pragma once

#if defined(__DragonFly__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(BSD) || defined(_SYSTYPE_BSD) ||           \
    defined(__OpenBSD__)
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <dlfcn.h>
#elif defined(__APPLE__) || defined(__DARWIN_C_LEVEL)
    #include <fcntl.h>
    #include <crt_externs.h>
#endif

#include <fast_io.h>
#include <fast_io_dsal/string.h>
#include <fast_io_dsal/string_view.h>
#include "abi.h"
#include "fd_map.h"
#include "../../../run/wasm_file.h"
#include "../../../clpara/parsing_result.h"
#include "../memory/memory.h"
#include "../wasm_exit.h"

#if (!defined(__NEWLIB__) || defined(__CYGWIN__)) && !defined(_WIN32) && __has_include(<dirent.h>) && !defined(_PICOLIBC__)
namespace uwvm::posix
{
    #if !defined(__MSDOS__)
    extern int fadvise(int fd, off_t offset, off_t len, int advice) noexcept __asm__("posix_fadvise");
    extern int fallocate(int fd, int mode, off_t offset, off_t len) noexcept __asm__("fallocate");
    #endif

    extern int fcntl(int fd, int cmd, ... /* arg */) noexcept
    #if !defined(__MSDOS__)
        __asm__("fcntl")
    #else
        __asm__("_fcntl")
    #endif
            ;
}  // namespace uwvm::posix
#endif

namespace uwvm::vm::interpreter::wasi
{
    ::std::int_least32_t args_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        auto argv_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg0))};

        auto const pr_cend{::uwvm::parsing_result.cend()};
        if(static_cast<::std::size_t>(memory_end - argv_begin) <
               sizeof(::std::uint_least32_t) * static_cast<::std::size_t>(pr_cend - (::uwvm::parsing_result.cbegin() + ::uwvm::wasm_file_ppos)) ||
           argv_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        using uint_least32_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::std::uint_least32_t*;

        auto argv_i32p{reinterpret_cast<uint_least32_t_may_alias_ptr>(argv_begin)};

        using char8_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = char8_t*;

        auto argv_buf_begin{reinterpret_cast<char8_t_may_alias_ptr>(memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1)))};

        for(auto i{::uwvm::parsing_result.cbegin() + ::uwvm::wasm_file_ppos}; i < pr_cend; ++i)
        {
            auto const argv_buf_offset_le32{
                ::fast_io::little_endian(static_cast<::std::uint_least32_t>(argv_buf_begin - reinterpret_cast<char8_t_may_alias_ptr>(memory_begin)))};
            ::fast_io::freestanding::my_memcpy(argv_i32p, __builtin_addressof(argv_buf_offset_le32), sizeof(argv_buf_offset_le32));
            auto const length{i->str.size()};

            if(static_cast<::std::size_t>(memory_end - reinterpret_cast<::std::byte*>(argv_buf_begin)) < length ||
               reinterpret_cast<::std::byte*>(argv_buf_begin) > memory_end) [[unlikely]]
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
            }

            ::fast_io::freestanding::my_memcpy(argv_buf_begin, i->str.data(), length);

            ++argv_i32p;
            argv_buf_begin += length;
            *argv_buf_begin++ = u8'\0';
        }

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t args_sizes_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};
        auto const pr_cend{::uwvm::parsing_result.cend()};

        // argc begin
        auto const argc_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg0))};

        if(static_cast<::std::size_t>(memory_end - argc_begin) < sizeof(::uwvm::vm::interpreter::wasi::wasi_size_t) || argc_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        using wasi_size_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::interpreter::wasi::wasi_size_t*;

        auto argv_i32p{reinterpret_cast<wasi_size_t_may_alias_ptr>(argc_begin)};

        auto const argv_wasm{static_cast<::uwvm::vm::interpreter::wasi::wasi_size_t>(pr_cend - (::uwvm::parsing_result.cbegin() + ::uwvm::wasm_file_ppos))};

        auto const argv_wasm_le32{::fast_io::little_endian(static_cast<::std::uint_least32_t>(argv_wasm))};

        ::fast_io::freestanding::my_memcpy(argv_i32p, __builtin_addressof(argv_wasm_le32), sizeof(::std::uint_least32_t));

        // buf len
        auto const buf_len_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - buf_len_begin) < sizeof(::uwvm::vm::interpreter::wasi::wasi_size_t) || buf_len_begin > memory_end)
            [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::std::uint_least32_t buf_len{};

        for(auto i{::uwvm::parsing_result.cbegin() + ::uwvm::wasm_file_ppos}; i < pr_cend; ++i) { buf_len += i->str.size() + 1; }

        auto const buf_len_le32{::fast_io::little_endian(static_cast<::std::uint_least32_t>(buf_len))};

        ::fast_io::freestanding::my_memcpy(buf_len_begin, __builtin_addressof(buf_len), sizeof(::std::uint_least32_t));

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t environ_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        auto env_para_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg0))};

        auto const pr_cend{::uwvm::parsing_result.cend()};

        if(static_cast<::std::size_t>(memory_end - env_para_begin) <
               sizeof(::std::uint_least32_t) * static_cast<::std::size_t>(pr_cend - (::uwvm::parsing_result.cbegin() + ::uwvm::wasm_file_ppos)) ||
           env_para_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        using uint_least32_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::std::uint_least32_t*;

        [[maybe_unused]] auto env_para_i32p{reinterpret_cast<uint_least32_t_may_alias_ptr>(env_para_begin)};

        using char8_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = char8_t*;

        [[maybe_unused]] auto env_buf_begin{
            reinterpret_cast<char8_t_may_alias_ptr>(memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1)))};

#if defined(__linux__) || defined(__sun)

        ::fast_io::u8native_file envs{};

    #ifdef __cpp_exceptions
        try
    #endif
        {
            envs = ::fast_io::u8native_file{u8"/proc/self/environ", ::fast_io::open_mode::in};
        }
    #ifdef __cpp_exceptions
        catch(::fast_io::error e)
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }
    #endif

        auto const env_buf_end{::fast_io::operations::read_some(envs, env_buf_begin, reinterpret_cast<char8_t_may_alias_ptr>(memory_end))};
        for(; env_buf_begin < env_buf_end;)
        {
            auto const env_buf_offset_le32{
                ::fast_io::little_endian(static_cast<::std::uint_least32_t>(env_buf_begin - reinterpret_cast<char8_t_may_alias_ptr>(memory_begin)))};
            ::fast_io::freestanding::my_memcpy(env_para_i32p, __builtin_addressof(env_buf_offset_le32), sizeof(env_buf_offset_le32));

            auto const pos{::std::find(env_buf_begin, env_buf_end, u8'\0')};

            env_buf_begin = pos + 1;
            if(*env_buf_begin == u8'0') [[unlikely]] { break; }
            ++env_para_i32p;
        }

#elif defined(__DragonFly__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(BSD) || defined(_SYSTYPE_BSD)
    #if defined(__NetBSD__)
        int mib[4]{CTL_KERN, KERN_PROC_ARGS, -1, KERN_PROC_ENV};
    #else
        int mib[4]{CTL_KERN, KERN_PROC, KERN_PROC_ENV, -1};
    #endif
        ::std::size_t size{static_cast<::std::size_t>(memory_end - reinterpret_cast<::std::byte*>(env_buf_begin))};

        if(::fast_io::noexcept_call(::sysctl, mib, 4, env_buf_begin, __builtin_addressof(size), nullptr, 0) != 0) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        auto const env_buf_end{env_buf_begin + size};

        for(; env_buf_begin < env_buf_end;)
        {
            auto const env_buf_offset_le32{
                ::fast_io::little_endian(static_cast<::std::uint_least32_t>(env_buf_begin - reinterpret_cast<char8_t_may_alias_ptr>(memory_begin)))};
            ::fast_io::freestanding::my_memcpy(env_para_i32p, __builtin_addressof(env_buf_offset_le32), sizeof(env_buf_offset_le32));

            auto const pos{::std::find(env_buf_begin, env_buf_end, u8'\0')};

            env_buf_begin = pos + 1;
            if(*env_buf_begin == u8'0') [[unlikely]] { break; }
            ++env_para_i32p;
        }

#elif defined(__OpenBSD__)
        int mib[4]{CTL_KERN, KERN_PROC_ARGS, getpid(), KERN_PROC_ENV};

        ::std::size_t size{};

        if(::fast_io::noexcept_call(::sysctl, mib, 4, nullptr, __builtin_addressof(size), nullptr, 0) != 0) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        if(size > static_cast<::std::size_t>(memory_end - reinterpret_cast<::std::byte*>(env_buf_begin))) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        if(::fast_io::noexcept_call(::sysctl, mib, 4, env_buf_begin, __builtin_addressof(size), nullptr, 0) != 0) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        auto const env_buf_end{env_buf_begin + size};

        for(; env_buf_begin < env_buf_end;)
        {
            auto const env_buf_offset_le32{
                ::fast_io::little_endian(static_cast<::std::uint_least32_t>(env_buf_begin - reinterpret_cast<char8_t_may_alias_ptr>(memory_begin)))};
            ::fast_io::freestanding::my_memcpy(env_para_i32p, __builtin_addressof(env_buf_offset_le32), sizeof(env_buf_offset_le32));

            auto const pos{::std::find(env_buf_begin, env_buf_end, u8'\0')};

            env_buf_begin = pos + 1;
            if(*env_buf_begin == u8'0') [[unlikely]] { break; }
            ++env_para_i32p;
        }

#elif defined(__APPLE__) || defined(__DARWIN_C_LEVEL)
        char*** const envs_get{::fast_io::noexcept_call(::_NSGetEnviron)};
        auto envion{*envs_get};

        while(*envion)
        {
            auto const env_buf_offset_le32{
                ::fast_io::little_endian(static_cast<::std::uint_least32_t>(env_buf_begin - reinterpret_cast<char8_t_may_alias_ptr>(memory_begin)))};
            ::fast_io::freestanding::my_memcpy(env_para_i32p, __builtin_addressof(env_buf_offset_le32), sizeof(env_buf_offset_le32));

            auto const env_str{*envion};
            auto const env_str_len{::fast_io::cstr_len(env_str)};

            if(reinterpret_cast<::std::byte*>(env_buf_begin) + env_str_len >= memory_end) [[unlikely]]
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
            }

            ::fast_io::freestanding::my_memcpy(env_buf_begin, env_str, env_str_len);

            env_buf_begin += env_str_len;
            *env_buf_begin++ = u8'\0';

            ++env_para_i32p;
            ++envion;
        }

#elif defined(_WIN32)
    #if defined(_WIN32_WINDOWS)
        // CreateEnvironmentBlock function
        // Minimum supported client	Windows 2000 Professional [desktop apps only]
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
    #else
        auto c_peb{::fast_io::win32::nt::nt_get_current_peb()};
        auto const Environment{c_peb->ProcessParameters->Environment};
        auto const EnvironmentSize{c_peb->ProcessParameters->EnvironmentSize};
        decltype(auto) two_zero{u"\0\0"};
        auto const Environment_end{::std::search(Environment, Environment + EnvironmentSize, two_zero, two_zero + 2)};

        ::fast_io::u8string env_str{::fast_io::u8concat_fast_io(::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(Environment, Environment_end)))};

        if(env_str.size() > static_cast<::std::size_t>(memory_end - reinterpret_cast<::std::byte*>(env_buf_begin))) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::fast_io::freestanding::my_memcpy(env_buf_begin, env_str.data(), env_str.size());

        auto const env_buf_end{env_buf_begin + env_str.size()};

        for(; env_buf_begin < env_buf_end;)
        {
            auto const env_buf_offset_le32{
                ::fast_io::little_endian(static_cast<::std::uint_least32_t>(env_buf_begin - reinterpret_cast<char8_t_may_alias_ptr>(memory_begin)))};
            ::fast_io::freestanding::my_memcpy(env_para_i32p, __builtin_addressof(env_buf_offset_le32), sizeof(env_buf_offset_le32));

            auto const pos{::std::find(env_buf_begin, env_buf_end, u8'\0')};

            env_buf_begin = pos + 1;
            if(*env_buf_begin == u8'0') [[unlikely]] { break; }
            ++env_para_i32p;
        }

    #endif
#else
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
#endif
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t environ_sizes_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        bool success{true};

        ::std::uint_least32_t nenv_wasm{};
        ::std::uint_least32_t nenv_len_wasm{};

#if defined(__linux__) || defined(__sun)
        char8_t buffer[32768];

        ::fast_io::u8native_file envs{};

    #ifdef __cpp_exceptions
        try
    #endif
        {
            envs = ::fast_io::u8native_file{u8"/proc/self/environ", ::fast_io::open_mode::in};
            auto const read_end{::fast_io::operations::read_some(envs, buffer, buffer + 32767)};

            nenv_wasm = static_cast<::std::uint_least32_t>(::std::count(buffer, read_end, u8'\0'));
            nenv_len_wasm = static_cast<::std::uint_least32_t>(read_end - buffer);
        }
    #ifdef __cpp_exceptions
        catch(::fast_io::error e)
        {
            success = false;
        }
    #endif

#elif defined(__DragonFly__) || defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__NetBSD__) || defined(BSD) || defined(_SYSTYPE_BSD)
    #if defined(__NetBSD__)
        int mib[4]{CTL_KERN, KERN_PROC_ARGS, -1, KERN_PROC_ENV};
    #else
        int mib[4]{CTL_KERN, KERN_PROC, KERN_PROC_ENV, -1};
    #endif

        {
            char8_t buffer[32768];
            ::std::size_t size{32767};
            if(::fast_io::noexcept_call(::sysctl, mib, 4, buffer, __builtin_addressof(size), nullptr, 0) == 0) [[likely]]
            {
                nenv_wasm = static_cast<::std::uint_least32_t>(::std::count(buffer, buffer + size, u8'\0'));
                nenv_len_wasm = static_cast<::std::uint_least32_t>(size);
            }
            else { success = false; }
        }

#elif defined(__OpenBSD__)
        int mib[4]{CTL_KERN, KERN_PROC_ARGS, getpid(), KERN_PROC_ENV};

        ::std::size_t size{};

        if(::fast_io::noexcept_call(::sysctl, mib, 4, nullptr, __builtin_addressof(size), nullptr, 0) == 0) [[likely]]
        {
            if(size < 32767) [[likely]]
            {
                char8_t buffer[32768];
                if(::fast_io::noexcept_call(::sysctl, mib, 4, buffer, __builtin_addressof(size), nullptr, 0) == 0) [[likely]]
                {
                    nenv_wasm = static_cast<::std::uint_least32_t>(::std::count(buffer, buffer + size, u8'\0'));
                    nenv_len_wasm = static_cast<::std::uint_least32_t>(size);
                }
                else { success = false; }
            }
            else { success = false; }
        }
        else { success = false; }

#elif defined(__APPLE__) || defined(__DARWIN_C_LEVEL)
        char*** const envs_get{::fast_io::noexcept_call(::_NSGetEnviron)};
        auto envion{*envs_get};

        while(*envion)
        {
            ++nenv_wasm;

            auto const env_str{*envion};
            auto const env_str_len{::fast_io::cstr_len(env_str)};
            nenv_len_wasm += env_str_len + 1;

            ++envion;
        }

#elif defined(_WIN32)
    #if defined(_WIN32_WINDOWS)
        // CreateEnvironmentBlock function
        // Minimum supported client	Windows 2000 Professional [desktop apps only]
        success = false;
    #else
        auto c_peb{::fast_io::win32::nt::nt_get_current_peb()};
        auto const Environment{c_peb->ProcessParameters->Environment};
        auto const EnvironmentSize{c_peb->ProcessParameters->EnvironmentSize};

        decltype(auto) two_zero{u"\0\0"};

        auto const Environment_end{::std::search(Environment, Environment + EnvironmentSize, two_zero, two_zero + 2) + 1};

        // in order to get u8 environments length
        ::fast_io::u8string env_str{::fast_io::u8concat_fast_io(::fast_io::mnp::code_cvt(::fast_io::mnp::strvw(Environment, Environment_end)))};

        nenv_wasm = static_cast<::std::uint_least32_t>(::std::ranges::count(env_str, u8'\0'));
        nenv_len_wasm = static_cast<::std::uint_least32_t>(env_str.size());

    #endif
#else
        success = false;
#endif

        auto const nenv_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg0))};

        if(static_cast<::std::size_t>(memory_end - nenv_begin) < sizeof(::uwvm::vm::interpreter::wasi::wasi_size_t) || nenv_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        using wasi_size_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
            [[__gnu__::__may_alias__]]
#endif
            = ::uwvm::vm::interpreter::wasi::wasi_size_t*;

        auto nenv_i32p{reinterpret_cast<wasi_size_t_may_alias_ptr>(nenv_begin)};

        auto const nenv_wasm_le32{::fast_io::little_endian(static_cast<::std::uint_least32_t>(nenv_wasm))};

        ::fast_io::freestanding::my_memcpy(nenv_i32p, __builtin_addressof(nenv_wasm_le32), sizeof(::std::uint_least32_t));

        // buf len
        auto const nenv_len_wasm_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - nenv_len_wasm_begin) < sizeof(::uwvm::vm::interpreter::wasi::wasi_size_t) ||
           nenv_len_wasm_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        auto const nenv_len_le32{::fast_io::little_endian(static_cast<::std::uint_least32_t>(nenv_len_wasm))};
        ::fast_io::freestanding::my_memcpy(nenv_len_wasm_begin, __builtin_addressof(nenv_len_wasm), sizeof(::std::uint_least32_t));

        if(success) [[likely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess); }
        else { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }
    }

    ::std::int_least32_t clock_res_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        ::fast_io::posix_clock_id id{};
        switch(static_cast<::uwvm::vm::interpreter::wasi::clockid_t>(arg0))
        {
            case ::uwvm::vm::interpreter::wasi::clockid_t::clock_realtime:
            {
                id = ::fast_io::posix_clock_id::realtime;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::clockid_t::clock_monotonic:
            {
                id = ::fast_io::posix_clock_id::monotonic;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::clockid_t::clock_process_cputime_id:
            {
                id = ::fast_io::posix_clock_id::process_cputime_id;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::clockid_t::clock_thread_cputime_id:
            {
                id = ::fast_io::posix_clock_id::thread_cputime_id;
                break;
            }
            default:
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval);
            }
        }

        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        auto ts_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - ts_begin) < sizeof(::uwvm::vm::interpreter::wasi::timestamp_t) || ts_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::fast_io::unix_timestamp ts{};
#ifdef __cpp_exceptions
        try
#endif
        {
            ts = ::fast_io::posix_clock_getres(id);
        }
#ifdef __cpp_exceptions
        catch(::fast_io::error e)
        {
            constexpr auto ts_u64_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(1'000'000u))};  // 1ms
            ::fast_io::freestanding::my_memcpy(ts_begin, __builtin_addressof(ts_u64_le), sizeof(ts_u64_le));
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }
#endif
        constexpr ::std::uint_least64_t mul_factor{::fast_io::uint_least64_subseconds_per_second / 1'000'000'000u};

        auto const ts_u64_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(ts.seconds * 1'000'000'000u + ts.subseconds / mul_factor))};
        ::fast_io::freestanding::my_memcpy(ts_begin, __builtin_addressof(ts_u64_le), sizeof(ts_u64_le));

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t clock_time_get(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least32_t arg2) noexcept
    {
        ::fast_io::posix_clock_id id{};
        switch(static_cast<::uwvm::vm::interpreter::wasi::clockid_t>(arg0))
        {
            case ::uwvm::vm::interpreter::wasi::clockid_t::clock_realtime:
            {
                id = ::fast_io::posix_clock_id::realtime;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::clockid_t::clock_monotonic:
            {
                id = ::fast_io::posix_clock_id::monotonic;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::clockid_t::clock_process_cputime_id:
            {
                id = ::fast_io::posix_clock_id::process_cputime_id;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::clockid_t::clock_thread_cputime_id:
            {
                id = ::fast_io::posix_clock_id::thread_cputime_id;
                break;
            }
            default:
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval);
            }
        }

        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        auto ts_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2))};

        if(static_cast<::std::size_t>(memory_end - ts_begin) < sizeof(::uwvm::vm::interpreter::wasi::timestamp_t) || ts_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::fast_io::unix_timestamp ts{};
#ifdef __cpp_exceptions
        try
#endif
        {
            ts = ::fast_io::posix_clock_gettime(id);
        }
#ifdef __cpp_exceptions
        catch(::fast_io::error e)
        {
            constexpr ::std::uint_least64_t ts_u64_le{};
            ::fast_io::freestanding::my_memcpy(ts_begin, __builtin_addressof(ts_u64_le), sizeof(ts_u64_le));
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }
#endif
        constexpr ::std::uint_least64_t mul_factor{::fast_io::uint_least64_subseconds_per_second / 1'000'000'000u};

        auto const ts_u64_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(ts.seconds * 1'000'000'000u + ts.subseconds / mul_factor))};
        ::fast_io::freestanding::my_memcpy(ts_begin, __builtin_addressof(ts_u64_le), sizeof(ts_u64_le));

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_advise(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least64_t arg2, ::std::int_least32_t arg3) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

#if (!defined(__NEWLIB__) || defined(__CYGWIN__)) && !defined(_WIN32) && !defined(__MSDOS__) && __has_include(<dirent.h>) && !defined(_PICOLIBC__)
    #if defined(__linux__) && defined(__NR_fadvise64)
        int advice{};
        switch(static_cast<::uwvm::vm::interpreter::wasi::advice_t>(arg3))
        {
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_normal:
            {
                advice = POSIX_FADV_NORMAL;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_sequential:
            {
                advice = POSIX_FADV_SEQUENTIAL;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_random:
            {
                advice = POSIX_FADV_RANDOM;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_willneed:
            {
                advice = POSIX_FADV_WILLNEED;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_dontneed:
            {
                advice = POSIX_FADV_DONTNEED;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_noreuse:
            {
                advice = POSIX_FADV_NOREUSE;
                break;
            }
            default:
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval);
            }
        }
        auto const rt{::fast_io::system_call<__NR_fadvise64, int>(fd, static_cast<off_t>(arg1), static_cast<off_t>(arg2), advice)};
        if(rt == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }
        else { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess); }
    #elif _POSIX_C_SOURCE >= 200112L
        int advice{};
        switch(static_cast<::uwvm::vm::interpreter::wasi::advice_t>(arg3))
        {
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_normal:
            {
                advice = POSIX_FADV_NORMAL;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_sequential:
            {
                advice = POSIX_FADV_SEQUENTIAL;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_random:
            {
                advice = POSIX_FADV_RANDOM;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_willneed:
            {
                advice = POSIX_FADV_WILLNEED;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_dontneed:
            {
                advice = POSIX_FADV_DONTNEED;
                break;
            }
            case ::uwvm::vm::interpreter::wasi::advice_t::advice_noreuse:
            {
                advice = POSIX_FADV_NOREUSE;
                break;
            }
            default:
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval);
            }
        }
        auto const rt{::uwvm::posix::fadvise(fd, static_cast<off_t>(arg1), static_cast<off_t>(arg2), advice)};
        if(rt == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }
        else { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess); }
    #else
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
    #endif
#else
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
#endif
    }

    ::std::int_least32_t fd_allocate(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least64_t arg2) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

#if (!defined(__NEWLIB__) || defined(__CYGWIN__)) && !defined(_WIN32) && !defined(__MSDOS__) &&                                                                \
    __has_include(<dirent.h>) && !defined(_PICOLIBC__) && !defined(__wasi__) && !defined(__DragonFly__) && !defined(__FreeBSD__) &&                            \
    !defined(__FreeBSD_kernel__) && !defined(__NetBSD__) && !defined(BSD) &&!defined(_SYSTYPE_BSD)  && !defined(__OpenBSD__)
    #if defined(__linux__) && defined(__NR_fallocate)
        auto const rt{::fast_io::system_call<__NR_fallocate, int>(fd, 0, static_cast<off_t>(arg1), static_cast<off_t>(arg2))};
        if(rt == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }
        else { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess); }
    #else
        auto const rt{::uwvm::posix::fallocate(fd, 0, static_cast<off_t>(arg1), static_cast<off_t>(arg2))};
        if(rt == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }
        else { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess); }
    #endif
#else
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
#endif
    }

    ::std::int_least32_t fd_close(::std::int_least32_t arg0) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto const rt{::fast_io::details::sys_close(fd)};
        if(rt == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }

        delete_wasm_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0);

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_datasync(::std::int_least32_t arg0) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

#if (defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__)) && !defined(__CYGWIN__)
        auto const rt{::fast_io::noexcept_call(_commit, fd)};
#elif defined(__linux__) && defined(__NR_fdatasync)
        auto const rt{::fast_io::system_call<__NR_fdatasync, int>(fd)};
#elif !defined(__MSDOS__)
        auto const rt{::fast_io::noexcept_call(fdatasync, fd)};
#else
        // fdatasync is temporarily not supported in msdosdjgpp
        auto const rt{::fast_io::noexcept_call(fsync, fd)};
#endif

        if(rt == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_sync(::std::int_least32_t arg0) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

#if (defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__)) && !defined(__CYGWIN__)
        auto const rt{::fast_io::noexcept_call(_commit, fd)};
#elif defined(__linux__) && defined(__NR_fsync)
        auto const rt{::fast_io::system_call<__NR_fsync, int>(fd)};
#else
        auto const rt{::fast_io::noexcept_call(fsync, fd)};
#endif

        if(rt == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_fdstat_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        // write pos begin
        auto const fdstat_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - fdstat_begin) < 24 || fdstat_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::uwvm::vm::interpreter::wasi::filetype_t fs_filetype{};                                                                 // 0
        ::uwvm::vm::interpreter::wasi::fdflags_t fs_flags{};                                                                     // 2
        ::uwvm::vm::interpreter::wasi::rights_t fs_rights_base{static_cast<::uwvm::vm::interpreter::wasi::rights_t>(-1)};        // 8
        ::uwvm::vm::interpreter::wasi::rights_t fs_rights_inheriting{static_cast<::uwvm::vm::interpreter::wasi::rights_t>(-1)};  // 16

        ::fast_io::posix_io_observer fpio{fd};

        ::fast_io::posix_file_status s{};

#ifdef __cpp_exceptions
        try
#endif
        {
            s = ::fast_io::status(fpio);
        }
#ifdef __cpp_exceptions
        catch(::fast_io::error e)
        {
            ::fast_io::freestanding::my_memset(fdstat_begin, 0, 24);
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }
#endif

        switch(s.type)
        {
            case ::fast_io::file_type::none:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            case ::fast_io::file_type::not_found:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            case ::fast_io::file_type::regular:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_regular_file;
                break;
            }
            case ::fast_io::file_type::directory:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_directory;
                break;
            }
            case ::fast_io::file_type::symlink:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_symbolic_link;
                break;
            }
            case ::fast_io::file_type::block:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_block_device;
                break;
            }
            case ::fast_io::file_type::character:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_character_device;
                break;
            }
            case ::fast_io::file_type::fifo:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            case ::fast_io::file_type::socket:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_socket_stream;
                break;
            }
            case ::fast_io::file_type::unknown:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            case ::fast_io::file_type::remote:
            {
                fs_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            default:
                [[unlikely]] { ::fast_io::fast_terminate(); }
        }

        ::std::uint_least16_t fs_flags_temp{};
#ifdef O_APPEND
        if(s.flags & O_APPEND) { fs_flags_temp |= static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_append); }
#endif
#ifdef O_DSYNC
        if(s.flags & O_DSYNC) { fs_flags_temp |= static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_dsync); }
#endif
#ifdef O_NONBLOCK
        if(s.flags & O_NONBLOCK) { fs_flags_temp |= static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_nonblock); }
#endif
#ifdef O_RSYNC
        if(s.flags & O_RSYNC) { fs_flags_temp |= static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_rsync); }
#endif
#ifdef O_SYNC
        if(s.flags & O_SYNC) { fs_flags_temp |= static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_sync); }
#endif
        fs_flags = static_cast<::uwvm::vm::interpreter::wasi::fdflags_t>(fs_flags_temp);

        if(fd < 3)
        {
            auto fs_rights_inheriting_temp{static_cast<::std::uint_least64_t>(fs_rights_inheriting)};
            fs_rights_inheriting_temp &= ~(static_cast<::std::uint_least64_t>(::uwvm::vm::interpreter::wasi::rights_t::right_fd_seek) |
                                           static_cast<::std::uint_least64_t>(::uwvm::vm::interpreter::wasi::rights_t::right_fd_tell));
            fs_rights_inheriting = static_cast<::uwvm::vm::interpreter::wasi::rights_t>(fs_rights_inheriting_temp);
        }

        ::fast_io::freestanding::my_memcpy(fdstat_begin, __builtin_addressof(fs_filetype), sizeof(fs_filetype));

        auto const fs_flags_le{::fast_io::little_endian(static_cast<::std::uint_least16_t>(fs_flags))};
        ::fast_io::freestanding::my_memcpy(fdstat_begin + 2, __builtin_addressof(fs_flags_le), sizeof(fs_flags_le));

        auto const fs_rights_base_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(fs_rights_base))};
        ::fast_io::freestanding::my_memcpy(fdstat_begin + 8, __builtin_addressof(fs_rights_base_le), sizeof(fs_rights_base_le));

        auto const fs_rights_inheriting_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(fs_rights_inheriting))};
        ::fast_io::freestanding::my_memcpy(fdstat_begin + 16, __builtin_addressof(fs_rights_inheriting_le), sizeof(fs_rights_inheriting_le));

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_fdstat_set_flags(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto const flags{static_cast<::uwvm::vm::interpreter::wasi::fdflags_t>(arg1)};
        auto const flags_u16{static_cast<::std::uint_least16_t>(flags)};

#if (defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__)) && !defined(__CYGWIN__)
        auto const win32_handle{static_cast<::fast_io::win32_io_observer>(::fast_io::posix_io_observer{fd})};
        void* const handle{win32_handle.native_handle()};

        ::fast_io::win32::by_handle_file_information bhfi{};
        if(!::fast_io::win32::GetFileInformationByHandle(handle, __builtin_addressof(bhfi))) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        if((flags_u16 & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_append)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_append))
        {
            bhfi.dwFileAttributes &= 0x0004 /* FILE_APPEND_DATA */;
        }
        else { bhfi.dwFileAttributes &= ~0x0004 /* FILE_APPEND_DATA */; }

        ::fast_io::win32::file_basic_info fbi{.FileAttributes{bhfi.dwFileAttributes}};

        if(!::fast_io::win32::SetFileInformationByHandle(handle,
                                                         ::fast_io::win32::file_info_by_handle_class::FileBasicInfo,
                                                         __builtin_addressof(fbi),
                                                         sizeof(fbi))) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }
#else
        ::std::uintmax_t fd_flags{};
        if((flags_u16 & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_append)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_append))
        {
    #ifdef O_APPEND
            fd_flags |= O_APPEND;
    #endif
        }
        if((flags_u16 & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_dsync)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_dsync))
        {
    #ifdef O_DSYNC
            fd_flags |= O_DSYNC;
    #endif
        }
        if((flags_u16 & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_nonblock)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_nonblock))
        {
    #ifdef O_NONBLOCK
            fd_flags |= O_NONBLOCK;
    #endif
        }
        if((flags_u16 & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_rsync)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_rsync))
        {
    #ifdef O_RSYNC
            fd_flags |= O_RSYNC;
    #endif
        }
        if((flags_u16 & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_sync)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fdflags_t::fdflag_sync))
        {
    #ifdef O_SYNC
            fd_flags |= O_SYNC;
    #endif
        }

        if(
    #if defined(__linux__) && defined(__NR_fcntl)
            ::fast_io::system_call<__NR_fcntl, int>(fd, F_SETFL, fd_flags)
    #else
            ::uwvm::posix::fcntl(fd, F_SETFL, fd_flags)
    #endif
            != 0) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }
#endif

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_fdstat_set_rights(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least64_t arg2) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
#if 0
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
#endif
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }
        // No support required
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
    }

    ::std::int_least32_t fd_filestat_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        // write pos begin
        auto const file_stat_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - file_stat_begin) < 64 || file_stat_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::uwvm::vm::interpreter::wasi::device_t st_dev{};
        ::uwvm::vm::interpreter::wasi::inode_t st_ino{};
        ::uwvm::vm::interpreter::wasi::filetype_t st_filetype{};
        ::uwvm::vm::interpreter::wasi::linkcount_t st_nlink{};
        ::uwvm::vm::interpreter::wasi::filesize_t st_size{};
        ::uwvm::vm::interpreter::wasi::timestamp_t st_atim{};
        ::uwvm::vm::interpreter::wasi::timestamp_t st_mtim{};
        ::uwvm::vm::interpreter::wasi::timestamp_t st_ctim{};

        ::fast_io::posix_io_observer fpio{fd};

        ::fast_io::posix_file_status s{};

#ifdef __cpp_exceptions
        try
#endif
        {
            s = ::fast_io::status(fpio);
        }
#ifdef __cpp_exceptions
        catch(::fast_io::error e)
        {
            ::fast_io::freestanding::my_memset(file_stat_begin, 0, 64);
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }
#endif

        st_dev = static_cast<::uwvm::vm::interpreter::wasi::device_t>(s.dev);
        st_ino = static_cast<::uwvm::vm::interpreter::wasi::inode_t>(s.ino);

        switch(s.type)
        {
            case ::fast_io::file_type::none:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            case ::fast_io::file_type::not_found:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            case ::fast_io::file_type::regular:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_regular_file;
                break;
            }
            case ::fast_io::file_type::directory:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_directory;
                break;
            }
            case ::fast_io::file_type::symlink:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_symbolic_link;
                break;
            }
            case ::fast_io::file_type::block:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_block_device;
                break;
            }
            case ::fast_io::file_type::character:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_character_device;
                break;
            }
            case ::fast_io::file_type::fifo:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            case ::fast_io::file_type::socket:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_socket_stream;
                break;
            }
            case ::fast_io::file_type::unknown:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            case ::fast_io::file_type::remote:
            {
                st_filetype = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                break;
            }
            default:
                [[unlikely]] { ::fast_io::fast_terminate(); }
        }

        st_nlink = static_cast<::uwvm::vm::interpreter::wasi::linkcount_t>(s.nlink);
        st_size = static_cast<::uwvm::vm::interpreter::wasi::filesize_t>(s.size);

        constexpr ::std::uint_least64_t mul_factor{::fast_io::uint_least64_subseconds_per_second / 1'000'000'000u};

        st_atim = static_cast<::uwvm::vm::interpreter::wasi::timestamp_t>(
            static_cast<::std::uint_least64_t>(s.atim.seconds * 1'000'000'000u + s.atim.subseconds / mul_factor));
        st_mtim = static_cast<::uwvm::vm::interpreter::wasi::timestamp_t>(
            static_cast<::std::uint_least64_t>(s.mtim.seconds * 1'000'000'000u + s.mtim.subseconds / mul_factor));
        st_ctim = static_cast<::uwvm::vm::interpreter::wasi::timestamp_t>(
            static_cast<::std::uint_least64_t>(s.ctim.seconds * 1'000'000'000u + s.ctim.subseconds / mul_factor));

        auto const st_dev_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(st_dev))};
        ::fast_io::freestanding::my_memcpy(file_stat_begin, __builtin_addressof(st_dev_le), sizeof(st_dev_le));

        auto const st_ino_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(st_ino))};
        ::fast_io::freestanding::my_memcpy(file_stat_begin + 8, __builtin_addressof(st_ino_le), sizeof(st_ino_le));

        ::fast_io::freestanding::my_memcpy(file_stat_begin + 16, __builtin_addressof(st_filetype), sizeof(st_filetype));

        auto const st_nlink_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(st_nlink))};
        ::fast_io::freestanding::my_memcpy(file_stat_begin + 24, __builtin_addressof(st_nlink_le), sizeof(st_nlink_le));

        auto const st_size_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(st_size))};
        ::fast_io::freestanding::my_memcpy(file_stat_begin + 32, __builtin_addressof(st_size_le), sizeof(st_size_le));

        auto const st_atim_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(st_atim))};
        ::fast_io::freestanding::my_memcpy(file_stat_begin + 40, __builtin_addressof(st_atim_le), sizeof(st_atim_le));

        auto const st_mtim_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(st_mtim))};
        ::fast_io::freestanding::my_memcpy(file_stat_begin + 48, __builtin_addressof(st_mtim_le), sizeof(st_mtim_le));

        auto const st_ctim_le{::fast_io::little_endian(static_cast<::std::uint_least64_t>(st_ctim))};
        ::fast_io::freestanding::my_memcpy(file_stat_begin + 56, __builtin_addressof(st_ctim_le), sizeof(st_ctim_le));

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_filestat_set_size(::std::int_least32_t arg0, ::std::int_least64_t arg1) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }
#if (defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__)) && !defined(__CYGWIN__)
        auto const rt{_chsize_s(fd, arg1)};
        if(rt == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }
#else
    #if defined(__linux__) && defined(__NR_ftruncate)
        auto const rt{::fast_io::system_call<__NR_ftruncate, int>(fd, static_cast<off_t>(arg1))};
    #else
        auto const rt{::fast_io::noexcept_call(ftruncate, fd, static_cast<off_t>(arg1))};
    #endif
        if(rt == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }
#endif

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t
        fd_filestat_set_times(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least64_t arg2, ::std::int_least32_t arg3) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto const flags{static_cast<::uwvm::vm::interpreter::wasi::fstflags_t>(arg3)};
        auto const atm{static_cast<::std::uint_least64_t>(arg1)};
        auto const mtm{static_cast<::std::uint_least64_t>(arg2)};

#if (!defined(__NEWLIB__) || defined(__CYGWIN__)) && !defined(_WIN32) && !defined(__MSDOS__) && __has_include(<dirent.h>) && !defined(_PICOLIBC__)
        struct timespec timestamp_spec[2];

        if((static_cast<::std::uint_least16_t>(flags) & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_atim_now)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_atim_now))
        {
            timestamp_spec[0] = {{}, UTIME_NOW};
        }
        else if((static_cast<::std::uint_least16_t>(flags) &
                 static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_atim)) ==
                static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_atim))
        {
            timestamp_spec[0] = {static_cast<::std::time_t>(atm / 1'000'000'000u), static_cast<long>(atm % 1'000'000'000u)};
        }
        else { timestamp_spec[0] = {{}, UTIME_OMIT}; }

        if((static_cast<::std::uint_least16_t>(flags) & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_mtim_now)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_mtim_now))
        {
            timestamp_spec[1] = {{}, UTIME_NOW};
        }
        else if((static_cast<::std::uint_least16_t>(flags) &
                 static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_mtim)) ==
                static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_mtim))
        {
            timestamp_spec[1] = {static_cast<::std::time_t>(mtm / 1'000'000'000u), static_cast<long>(mtm % 1'000'000'000u)};
        }
        else { timestamp_spec[1] = {{}, UTIME_OMIT}; }

        if(::fast_io::noexcept_call(futimens, fd, timestamp_spec) == -1) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

#elif defined(_WIN32)

        constexpr auto uts_to_file_time = [](::std::uint_least64_t uts) noexcept -> ::fast_io::win32::filetime
        {
            constexpr ::std::uint_least64_t gap{11644473600000ULL * 10000ULL};
            auto const date_time{uts + gap};

            return ::fast_io::win32::filetime{.dwLowDateTime{static_cast<::std::uint_least32_t>(date_time)},
                                              .dwHighDateTime{static_cast<::std::uint_least32_t>(date_time >> 32u)}};
        };

        auto const win32_handle{static_cast<::fast_io::win32_io_observer>(::fast_io::posix_io_observer{fd})};
        void* const handle{win32_handle.native_handle()};

        ::fast_io::win32::filetime currtime{};

        ::fast_io::win32::filetime lpLastAccessTime{};
        ::fast_io::win32::filetime lpLastWriteTime{};

        if((static_cast<::std::uint_least16_t>(flags) & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_atim_now)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_atim_now))
        {
    #if (!defined(_WIN32_WINNT) || _WIN32_WINNT >= 0x0602) && !defined(_WIN32_WINDOWS)
            ::fast_io::win32::GetSystemTimePreciseAsFileTime(__builtin_addressof(currtime));
    #else
            ::fast_io::win32::GetSystemTimeAsFileTime(__builtin_addressof(currtime));
    #endif
            lpLastAccessTime = currtime;
        }
        else if((static_cast<::std::uint_least16_t>(flags) &
                 static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_atim)) ==
                static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_atim))
        {
            lpLastAccessTime = uts_to_file_time(atm);
        }

        if((static_cast<::std::uint_least16_t>(flags) & static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_mtim_now)) ==
           static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_mtim_now))
        {
            if(::std::bit_cast<::std::uint_least64_t>(currtime) == 0)
            {
    #if (!defined(_WIN32_WINNT) || _WIN32_WINNT >= 0x0602) && !defined(_WIN32_WINDOWS)
                ::fast_io::win32::GetSystemTimePreciseAsFileTime(__builtin_addressof(currtime));
    #else
                ::fast_io::win32::GetSystemTimeAsFileTime(__builtin_addressof(currtime));
    #endif
            }
            lpLastWriteTime = currtime;
        }
        else if((static_cast<::std::uint_least16_t>(flags) &
                 static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_mtim)) ==
                static_cast<::std::uint_least16_t>(::uwvm::vm::interpreter::wasi::fstflags_t::filestat_set_mtim))
        {
            lpLastWriteTime = uts_to_file_time(mtm);
        }

        if(!::fast_io::win32::SetFileTime(handle, nullptr, __builtin_addressof(lpLastAccessTime), __builtin_addressof(lpLastWriteTime))) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

#else
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
#endif
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_prestat_get(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
#if 0
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
#endif
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }
        // No support required
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
    }

    ::std::int_least32_t fd_prestat_dir_name(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
#if 0
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
#endif
        auto const fd{gfd.fd};

        if(fd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }
        // No support required
        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
    }

    ::std::int_least32_t
        fd_pread(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2, ::std::int_least64_t arg3, ::std::int_least32_t arg4) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const pfd{gfd.fd};

        if(pfd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        ::std::byte const* cvt_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - cvt_begin) < 8u * static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) ||
           cvt_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

#if __has_builtin(__builtin_alloca)
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            __builtin_alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#elif defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__) && !defined(__CYGWIN__)
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            _alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#else
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#endif

        ::std::size_t all_read_size{};

        for(::std::size_t i{}; i < static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)); ++i)
        {
            ::uwvm::vm::interpreter::wasi::wasi_void_ptr_t wpt_buf{};
            ::uwvm::vm::interpreter::wasi::wasi_size_t wsz_buf_len{};

            ::std::uint_least32_t le_wpt_buf{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(le_wpt_buf), cvt_begin, sizeof(le_wpt_buf));
            wpt_buf = static_cast<decltype(wpt_buf)>(::fast_io::little_endian(le_wpt_buf));
            cvt_begin += 4;

            ::std::uint_least32_t le_wsz_buf_len{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(le_wsz_buf_len), cvt_begin, sizeof(le_wsz_buf_len));
            wsz_buf_len = static_cast<decltype(wsz_buf_len)>(::fast_io::little_endian(le_wsz_buf_len));
            cvt_begin += 4;

            auto const cvt_buf_off{static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(wpt_buf))};
            auto const cvt_buf_begin{memory_begin + cvt_buf_off};

            if(static_cast<::std::size_t>(memory_end - cvt_buf_begin) < static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(wsz_buf_len)) ||
               cvt_buf_begin > memory_end) [[unlikely]]
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
            }

            auto& tmpi{tmp[i]};

            using char_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                [[__gnu__::__may_alias__]]
#endif
                = char const*;

            tmpi.base = reinterpret_cast<char_const_may_alias_ptr>(cvt_buf_begin);
            tmpi.len = static_cast<::std::size_t>(le_wsz_buf_len);

            all_read_size += static_cast<::std::size_t>(le_wsz_buf_len);
        }

        auto const [position,
                    position_in_scatter]{::fast_io::operations::scatter_pread_some(::fast_io::posix_io_observer{pfd},
                                                                                   tmp,
                                                                                   static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)),
                                                                                   static_cast<::fast_io::intfpos_t>(arg3))};

        ::std::byte* number_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg4))};

        if(static_cast<::std::size_t>(memory_end - number_begin) < sizeof(::std::uint_least32_t) || number_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::std::uint_least32_t le32{::fast_io::little_endian(static_cast<::std::uint_least32_t>(all_read_size))};
        ::fast_io::freestanding::my_memcpy(number_begin, __builtin_addressof(le32), sizeof(le32));

        if(position != static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) || position_in_scatter != 0) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_pwrite(::std::int_least32_t arg0,
                                   ::std::int_least32_t arg1,
                                   ::std::int_least32_t arg2,
                                   ::std::int_least64_t arg3,
                                   ::std::int_least32_t arg4) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const pfd{gfd.fd};

        if(pfd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        ::std::byte const* cvt_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - cvt_begin) < 8u * static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) ||
           cvt_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

#if __has_builtin(__builtin_alloca)
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            __builtin_alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#elif defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__) && !defined(__CYGWIN__)
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            _alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#else
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#endif

        ::std::size_t all_size{};

        for(::std::size_t i{}; i < static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)); ++i)
        {
            ::uwvm::vm::interpreter::wasi::wasi_void_ptr_t wpt_buf{};
            ::uwvm::vm::interpreter::wasi::wasi_size_t wsz_buf_len{};

            ::std::uint_least32_t le_wpt_buf{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(le_wpt_buf), cvt_begin, sizeof(le_wpt_buf));
            wpt_buf = static_cast<decltype(wpt_buf)>(::fast_io::little_endian(le_wpt_buf));
            cvt_begin += 4;

            ::std::uint_least32_t le_wsz_buf_len{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(le_wsz_buf_len), cvt_begin, sizeof(le_wsz_buf_len));
            wsz_buf_len = static_cast<decltype(wsz_buf_len)>(::fast_io::little_endian(le_wsz_buf_len));
            cvt_begin += 4;

            auto const cvt_buf_off{static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(wpt_buf))};
            auto const cvt_buf_begin{memory_begin + cvt_buf_off};

            if(static_cast<::std::size_t>(memory_end - cvt_buf_begin) < static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(wsz_buf_len)) ||
               cvt_buf_begin > memory_end) [[unlikely]]
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
            }

            auto& tmpi{tmp[i]};

            using char_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                [[__gnu__::__may_alias__]]
#endif
                = char const*;

            tmpi.base = reinterpret_cast<char_const_may_alias_ptr>(cvt_buf_begin);
            tmpi.len = static_cast<::std::size_t>(le_wsz_buf_len);

            all_size += static_cast<::std::size_t>(le_wsz_buf_len);
        }

        auto const [position,
                    position_in_scatter]{::fast_io::operations::scatter_pwrite_some(::fast_io::posix_io_observer{pfd},
                                                                                    tmp,
                                                                                    static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)),
                                                                                    static_cast<::fast_io::intfpos_t>(arg3))};

        ::std::byte* number_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg4))};

        if(static_cast<::std::size_t>(memory_end - number_begin) < sizeof(::std::uint_least32_t) || number_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::std::uint_least32_t le32{::fast_io::little_endian(static_cast<::std::uint_least32_t>(all_size))};
        ::fast_io::freestanding::my_memcpy(number_begin, __builtin_addressof(le32), sizeof(le32));

        if(position != static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) || position_in_scatter != 0) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_read(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2, ::std::int_least32_t arg3) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const pfd{gfd.fd};

        if(pfd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        ::std::byte const* cvt_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - cvt_begin) < 8u * static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) ||
           cvt_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

#if __has_builtin(__builtin_alloca)
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            __builtin_alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#elif defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__) && !defined(__CYGWIN__)
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            _alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#else
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#endif

        ::std::size_t all_size{};

        for(::std::size_t i{}; i < static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)); ++i)
        {
            ::uwvm::vm::interpreter::wasi::wasi_void_ptr_t wpt_buf{};
            ::uwvm::vm::interpreter::wasi::wasi_size_t wsz_buf_len{};

            ::std::uint_least32_t le_wpt_buf{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(le_wpt_buf), cvt_begin, sizeof(le_wpt_buf));
            wpt_buf = static_cast<decltype(wpt_buf)>(::fast_io::little_endian(le_wpt_buf));
            cvt_begin += 4;

            ::std::uint_least32_t le_wsz_buf_len{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(le_wsz_buf_len), cvt_begin, sizeof(le_wsz_buf_len));
            wsz_buf_len = static_cast<decltype(wsz_buf_len)>(::fast_io::little_endian(le_wsz_buf_len));
            cvt_begin += 4;

            auto const cvt_buf_off{static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(wpt_buf))};
            auto const cvt_buf_begin{memory_begin + cvt_buf_off};

            if(static_cast<::std::size_t>(memory_end - cvt_buf_begin) < static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(wsz_buf_len)) ||
               cvt_buf_begin > memory_end) [[unlikely]]
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
            }

            auto& tmpi{tmp[i]};

            using char_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                [[__gnu__::__may_alias__]]
#endif
                = char const*;

            tmpi.base = reinterpret_cast<char_const_may_alias_ptr>(cvt_buf_begin);
            tmpi.len = static_cast<::std::size_t>(le_wsz_buf_len);

            all_size += static_cast<::std::size_t>(le_wsz_buf_len);
        }

        auto const [position,
                    position_in_scatter]{::fast_io::operations::scatter_read_some(::fast_io::posix_io_observer{pfd},
                                                                                  tmp,
                                                                                  static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)))};

        ::std::byte* number_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg3))};

        if(static_cast<::std::size_t>(memory_end - number_begin) < sizeof(::std::uint_least32_t) || number_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::std::uint_least32_t le32{::fast_io::little_endian(static_cast<::std::uint_least32_t>(all_size))};
        ::fast_io::freestanding::my_memcpy(number_begin, __builtin_addressof(le32), sizeof(le32));

        if(position != static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) || position_in_scatter != 0) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    namespace details
    {
        template <typename T>
        concept wasi_entry_has_d_ino = requires(T t) { t.entry->d_ino; };

        template <typename T>
        inline ::uwvm::vm::interpreter::wasi::inode_t get_inode(T const& ent) noexcept
        {
            constexpr bool has_d_ino{details::wasi_entry_has_d_ino<T>};

            if constexpr(has_d_ino) { return static_cast<::uwvm::vm::interpreter::wasi::inode_t>(ent.entry->d_ino); }
            else
            {
                static ::std::uint_least64_t temp{};
                return static_cast<::uwvm::vm::interpreter::wasi::inode_t>(temp++);
            }
        }
    }  // namespace details

    ::std::int_least32_t fd_readdir(::std::int_least32_t arg0,
                                    ::std::int_least32_t arg1,
                                    ::std::int_least32_t arg2,
                                    ::std::int_least64_t arg3,
                                    ::std::int_least32_t arg4) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const pfd{gfd.fd};

        if(pfd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        ::std::byte const* const buf_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - buf_begin) < static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) || buf_begin > memory_end)
            [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::std::byte const* const buf_end{buf_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2))};

        ::std::byte* const number_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg4))};

        if(static_cast<::std::size_t>(memory_end - number_begin) < sizeof(::std::uint_least32_t) || number_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        auto const cookie{static_cast<::std::uint_least64_t>(arg3)};

        ::std::size_t cookie_counter{};
        ::std::byte* curr{const_cast<::std::byte*>(buf_begin)};

        for(auto const& ent: current(at(::fast_io::posix_io_observer{pfd})))
        {
            if(buf_end - curr < 24 || curr > buf_end) [[unlikely]]
            {
                ::fast_io::freestanding::my_memset(number_begin, 0, sizeof(::std::uint_least32_t));

                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
            }

            ::uwvm::vm::interpreter::wasi::dircookie_t const d_next{static_cast<::uwvm::vm::interpreter::wasi::dircookie_t>(cookie_counter)};

            if(cookie_counter++ < cookie) { continue; }

            ::uwvm::vm::interpreter::wasi::inode_t const d_ino{details::get_inode(ent)};

            ::fast_io::u8cstring_view const fn{u8filename(ent)};

            ::uwvm::vm::interpreter::wasi::dirnamlen_t const d_namlen{static_cast<::uwvm::vm::interpreter::wasi::dirnamlen_t>(fn.size())};

            ::uwvm::vm::interpreter::wasi::filetype_t d_type{};

            switch(
#if defined(_WIN32)
                ent.entry->d_type
#else
                ::fast_io::details::st_mode_to_file_type(ent.entry->d_type)
#endif
            )
            {
                case ::fast_io::file_type::none:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                    break;
                }
                case ::fast_io::file_type::not_found:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                    break;
                }
                case ::fast_io::file_type::regular:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_regular_file;
                    break;
                }
                case ::fast_io::file_type::directory:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_directory;
                    break;
                }
                case ::fast_io::file_type::symlink:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_symbolic_link;
                    break;
                }
                case ::fast_io::file_type::block:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_block_device;
                    break;
                }
                case ::fast_io::file_type::character:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_character_device;
                    break;
                }
                case ::fast_io::file_type::fifo:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                    break;
                }
                case ::fast_io::file_type::socket:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_socket_stream;
                    break;
                }
                case ::fast_io::file_type::unknown:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                    break;
                }
                case ::fast_io::file_type::remote:
                {
                    d_type = ::uwvm::vm::interpreter::wasi::filetype_t::filetype_unknown;
                    break;
                }
                default:
                    [[unlikely]] { ::fast_io::fast_terminate(); }
            }

            ::std::uint_least64_t const le_d_next{::fast_io::little_endian(static_cast<::std::uint_least64_t>(d_next))};
            ::fast_io::freestanding::my_memcpy(curr, __builtin_addressof(le_d_next), sizeof(le_d_next));
            curr += 8;

            ::std::uint_least64_t const le_d_ino{::fast_io::little_endian(static_cast<::std::uint_least64_t>(d_ino))};
            ::fast_io::freestanding::my_memcpy(curr, __builtin_addressof(le_d_ino), sizeof(le_d_ino));
            curr += 8;

            ::std::uint_least32_t const le_d_namlen{::fast_io::little_endian(static_cast<::std::uint_least32_t>(d_namlen))};
            ::fast_io::freestanding::my_memcpy(curr, __builtin_addressof(le_d_namlen), sizeof(le_d_namlen));
            curr += 4;

            ::fast_io::freestanding::my_memcpy(curr, __builtin_addressof(d_type), sizeof(d_type));
            curr += 4;
        }

        ::std::uint_least32_t num_used{static_cast<::std::uint_least32_t>(curr - buf_begin)};
        ::std::uint_least32_t const le_d_namlen{::fast_io::little_endian(num_used)};
        ::fast_io::freestanding::my_memcpy(number_begin, __builtin_addressof(le_d_namlen), sizeof(::std::uint_least32_t));

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_renumber(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept
    {
        if(arg0 == arg1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess); }

        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};

        auto const old_sys_fd{gfd.fd};

        // dup
        ::fast_io::posix_file temp_posix_file{::fast_io::io_dup, ::fast_io::posix_io_observer{old_sys_fd}};

        // get dup fd
        auto const new_sys_fd{temp_posix_file.release()};

        auto [r_success, r_old_sys_fd]{set_and_get_system_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg1, new_sys_fd)};

        if(!r_success) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault); }

        if(r_old_sys_fd != -1) [[likely]]
        {
            auto const ret{::fast_io::details::sys_close(r_old_sys_fd)};

#if defined(__linux__)
            using unsigned_t = ::std::make_unsigned_t<decltype(ret)>;
            if(static_cast<unsigned_t>(static_cast<unsigned_t>(ret) + static_cast<unsigned_t>(4096)) < static_cast<unsigned_t>(4096))
#else
            if(ret < 0)
#endif
                [[unlikely]]
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
            }
        }

        return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::esuccess);
    }

    ::std::int_least32_t fd_seek(::std::int_least32_t arg0, ::std::int_least64_t arg1, ::std::int_least32_t arg2, ::std::int_least32_t arg3) noexcept
    {
        return {};
    }

    ::std::int_least32_t fd_tell(::std::int_least32_t arg0, ::std::int_least32_t arg1) noexcept { return {}; }

    ::std::int_least32_t fd_write(::std::int_least32_t arg0, ::std::int_least32_t arg1, ::std::int_least32_t arg2, ::std::int_least32_t arg3) noexcept
    {
        auto const gfd{get_fd(::uwvm::vm::interpreter::wasi::wasm_fd_storages, arg0)};
        ::fast_io::io_lock_guard fd_look{*gfd.fd_mutex};
        auto const pfd{gfd.fd};

        if(pfd == -1) [[unlikely]] { return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::einval); }

        auto& memory{::uwvm::vm::interpreter::memories.front()};
        auto const memory_begin{memory.memory_begin};
        auto const memory_length{memory.memory_length};
        auto const memory_end{memory_begin + memory_length};

        ::std::byte const* cvt_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg1))};

        if(static_cast<::std::size_t>(memory_end - cvt_begin) < 8u * static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) ||
           cvt_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

#if __has_builtin(__builtin_alloca)
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            __builtin_alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#elif defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__) && !defined(__CYGWIN__)
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            _alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#else
        auto tmp{static_cast<::fast_io::basic_io_scatter_t<char>*>(
            alloca(static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) * sizeof(::fast_io::basic_io_scatter_t<char>)))};
#endif

        ::std::size_t all_write_size{};

        for(::std::size_t i{}; i < static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)); ++i)
        {
            ::uwvm::vm::interpreter::wasi::wasi_void_ptr_t wpt_buf{};
            ::uwvm::vm::interpreter::wasi::wasi_size_t wsz_buf_len{};

            ::std::uint_least32_t le_wpt_buf{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(le_wpt_buf), cvt_begin, sizeof(le_wpt_buf));
            wpt_buf = static_cast<decltype(wpt_buf)>(::fast_io::little_endian(le_wpt_buf));
            cvt_begin += 4;

            ::std::uint_least32_t le_wsz_buf_len{};
            ::fast_io::freestanding::my_memcpy(__builtin_addressof(le_wsz_buf_len), cvt_begin, sizeof(le_wsz_buf_len));
            wsz_buf_len = static_cast<decltype(wsz_buf_len)>(::fast_io::little_endian(le_wsz_buf_len));
            cvt_begin += 4;

            auto const cvt_buf_off{static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(wpt_buf))};
            auto const cvt_buf_begin{memory_begin + cvt_buf_off};

            if(static_cast<::std::size_t>(memory_end - cvt_buf_begin) < static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(wsz_buf_len)) ||
               cvt_buf_begin > memory_end) [[unlikely]]
            {
                return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
            }

            auto& tmpi{tmp[i]};

            using char_const_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                [[__gnu__::__may_alias__]]
#endif
                = char const*;

            tmpi.base = reinterpret_cast<char_const_may_alias_ptr>(cvt_buf_begin);
            tmpi.len = static_cast<::std::size_t>(le_wsz_buf_len);

            all_write_size += static_cast<::std::size_t>(le_wsz_buf_len);
        }

        auto const [position,
                    position_in_scatter]{::fast_io::operations::scatter_write_some(::fast_io::posix_io_observer{pfd},
                                                                                   tmp,
                                                                                   static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)))};

        ::std::byte* number_begin{memory_begin + static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg3))};

        if(static_cast<::std::size_t>(memory_end - number_begin) < sizeof(::std::uint_least32_t) || number_begin > memory_end) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

        ::std::uint_least32_t le32{::fast_io::little_endian(static_cast<::std::uint_least32_t>(all_write_size))};
        ::fast_io::freestanding::my_memcpy(number_begin, __builtin_addressof(le32), sizeof(le32));

        if(position != static_cast<::std::size_t>(static_cast<::std::uint_least32_t>(arg2)) || position_in_scatter != 0) [[unlikely]]
        {
            return static_cast<::std::int_least32_t>(::uwvm::vm::interpreter::wasi::errno_t::efault);
        }

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

    void proc_exit(::std::int_least32_t arg0) noexcept
    {
        ::uwvm::vm::interpreter::wasm_exit();

#if defined(__linux__) && defined(__NR_exit)
        ::fast_io::fast_exit(arg0);
#else
    #if __has_builtin(__builtin_exit)
        __builtin_exit(static_cast<int>(arg0));
    #else
        ::exit(static_cast<int>(arg0));
    #endif
#endif
    }

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
