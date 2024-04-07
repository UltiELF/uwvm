#pragma once

#if defined(__linux__)
#include <asm/unistd.h>
#ifdef __x86_64__
#include "amd64.h"
#elif defined(__arm64__) || defined(__aarch64__)
#include "aarch64.h"
#elif defined(__riscv) && __SIZEOF_SIZE_T__ == 8
#include "riscv64.h"
#elif defined(__loongarch__) && __SIZEOF_SIZE_T__ == 8
#include "loongarch64.h"
#else
#include "generic.h"
#endif
#endif

namespace fast_io
{

#if defined(__linux__)

template <::std::integral int_type>
inline constexpr bool linux_system_call_fails(int_type v) noexcept
{
	using unsigned_t = ::std::make_unsigned_t<int_type>;
	return static_cast<unsigned_t>(static_cast<unsigned_t>(v) + static_cast<unsigned_t>(4096)) <
		   static_cast<unsigned_t>(4096);
}

template <::std::integral I>
	requires(sizeof(I) >= 1)
inline void linux_system_call_throw_error(I v)
{
	using unsigned_t = ::std::make_unsigned_t<I>;
	if (static_cast<unsigned_t>(static_cast<unsigned_t>(v) + static_cast<unsigned_t>(4096)) <
		static_cast<unsigned_t>(4096))
	{
		throw_posix_error(static_cast<int>(-v));
	}
}

#endif

template <bool always_terminate = false, ::std::integral I>
	requires(sizeof(I) >= 1)
inline void system_call_throw_error(I v)
{
#if defined(__linux__)
	using unsigned_t = ::std::make_unsigned_t<I>;
	if (static_cast<unsigned_t>(static_cast<unsigned_t>(v) + static_cast<unsigned_t>(4096)) <
		static_cast<unsigned_t>(4096))
	{
		if constexpr (always_terminate)
		{
    #if defined(__has_builtin)
        #if __has_builtin(__builtin_perror)
                    __builtin_perror
        #else
                    ::perror
        #endif
    #else
                    ::perror
    #endif
                        (                           
                            "\033[0m"
    #ifdef __MSDOS__
                            "\033[37m"
    #else
                            "\033[97m"
    #endif
                            "uwvm: "
                            "\033[31m"
                            "[fatal] "
                            "\033[0m"
    #ifdef __MSDOS__
                            "\033[37m"
    #else
                            "\033[97m"
    #endif
                            "Trigger linux syscall error.\n"
                            "\033[0m"
                            "Terminate.\n\n");
              fast_terminate();
		}
		else
		{
			throw_posix_error(static_cast<int>(-v));
		}
	}
#else
	if (v < 0)
	{
		if constexpr (always_terminate)
		{
    #if defined(__has_builtin)
        #if __has_builtin(__builtin_perror)
                    __builtin_perror
        #else
                    ::perror
        #endif
    #else
                    ::perror
    #endif
                        (                           
                            "\033[0m"
    #ifdef __MSDOS__
                            "\033[37m"
    #else
                            "\033[97m"
    #endif
                            "uwvm: "
                            "\033[31m"
                            "[fatal] "
                            "\033[0m"
    #ifdef __MSDOS__
                            "\033[37m"
    #else
                            "\033[97m"
    #endif
                            "Trigger linux syscall error.\n"
                            "\033[0m"
                            "Terminate.\n\n");
			fast_terminate();
		}
		else
		{
			throw_posix_error();
		}
	}
#endif
}

} // namespace fast_io
