#pragma once

#include <cstdint>
#include <cstddef>
#include <concepts>
#include <compare>

#include <fast_io.h>

namespace uwvm
{

    struct version
    {
        ::std::uint_least32_t x{};
        ::std::uint_least32_t y{};
        ::std::uint_least32_t z{};
        ::std::uint_least32_t state{};
    };

    constexpr inline bool operator== (version v1, version v2) noexcept
    {
#if 0
    #ifdef _DEBUG
    return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.state == v2.state;
    #else
    if constexpr (4 * sizeof(::std::uint_least32_t) == sizeof(version)) {
        #if __cpp_if_consteval >= 202106L
        if consteval
        #else
        if (__builtin_is_constant_evaluated())
        #endif
        {
            return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.state == v2.state;
        } else {
            return ::fast_io::freestanding::my_memcmp(__builtin_addressof(v2), __builtin_addressof(v1), sizeof(version)) == 0;
        }
    } else {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.state == v2.state;
    }
    #endif  // _DEBUG
#else
        return v1.x == v2.x & v1.y == v2.y & v1.z == v2.z & v1.state == v2.state;
        /* x86_64 clang
                xor     rdi, rdx
                xor     rsi, rcx
                or      rsi, rdi
                sete    al
                ret
        */
        /* x86_64 gcc
                cmp     edi, edx
                sete    al
                shr     rdi, 32
                shr     rdx, 32
                cmp     edi, edx
                sete    dl
                and     eax, edx
                cmp     esi, ecx
                sete    dl
                shr     rsi, 32
                shr     rcx, 32
                cmp     esi, ecx
                sete    cl
                and     edx, ecx
                and     eax, edx
                ret
        */
#endif
    }

    constexpr inline ::std::strong_ordering operator<=> (version v1, version v2) noexcept
    {
        const auto cx{v1.x <=> v2.x};
        if(cx == 0)
        {
            const auto cy{v1.y <=> v2.y};
            if(cy == 0)
            {
                const auto cz{v1.z <=> v2.z};
                if(cz == 0) { return v1.state <=> v2.state; }
                return cz;
            }
            return cy;
        }
        return cx;
    }

    template <::std::integral char_type>
        requires (sizeof(char_type) == sizeof(char8_t))
    constexpr inline ::std::size_t print_reserve_size(::fast_io::io_reserve_type_t<char_type, version>) noexcept
    {
        constexpr ::std::size_t real_size{print_reserve_size(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>)};
        constexpr ::std::size_t size{3 + 4 * real_size};
        return size;
    }

    namespace details
    {
        template <::std::integral char_type>
        constexpr inline char_type* version_print_reserve_impl(char_type* iter,
                                                               ::std::uint_least32_t x,
                                                               ::std::uint_least32_t y,
                                                               ::std::uint_least32_t z,
                                                               ::std::uint_least32_t state) noexcept
        {
            char_type* curr_pos{print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>, iter, x)};
            *(curr_pos++) = static_cast<char_type>('.');
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>, curr_pos, y);
            *(curr_pos++) = static_cast<char_type>('.');
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>, curr_pos, z);
            *(curr_pos++) = static_cast<char_type>('.');
            curr_pos = print_reserve_define(::fast_io::io_reserve_type<char_type, ::std::uint_least32_t>, curr_pos, state);
            return curr_pos;
        }
    }  // namespace details

    template <::std::integral char_type>
    constexpr inline char_type* print_reserve_define(::fast_io::io_reserve_type_t<char_type, version>, char_type* iter, version ver) noexcept
    {
        return details::version_print_reserve_impl(iter, ver.x, ver.y, ver.z, ver.state);
    }

#if defined(UWVM_VERSION_X) && defined(UWVM_VERSION_Y) && defined(UWVM_VERSION_Z) && defined(UWVM_VERSION_S)
    constexpr inline version uwvm_version{UWVM_VERSION_X, UWVM_VERSION_Y, UWVM_VERSION_Z, UWVM_VERSION_S};
    constexpr inline ::fast_io::mnp::basic_os_c_str_with_known_size<char8_t> git_fetch_head{::fast_io::mnp::os_c_str_arr(
    #include "../../.tmp/git_commit_hash.h"
        )};
#else
    constexpr inline version uwvm_version{};
    constexpr inline ::fast_io::mnp::basic_os_c_str_with_known_size<char8_t> git_fetch_head{::fast_io::mnp::os_c_str_arr(u8"No FETCH_HEAD information")};
#endif

}  // namespace uwvm
