#pragma once
#include <fast_io.h>
#include <fast_io_crypto.h>
#include <fast_io_dsal/string_view.h>
#include <fast_io_dsal/array.h>
#include "import.h"

namespace uwvm::vm::unchecked_interpreter
{

    struct import_pair
    {
        ::fast_io::u8string_view str{};
        ::uwvm::vm::unchecked_interpreter::int_import_func_p func_p{};
    };

    inline constexpr ::std::size_t hash_size_base{4u};  // 2 ^ hash_size_base
    inline constexpr ::std::size_t max_conflict_size{8u};

    template <::std::size_t N>
    inline
#if __cpp_consteval >= 201811L
        consteval
#else
        constexpr
#endif
        auto
        check_import(import_pair const (&ip)[N]) noexcept
    {
        ::fast_io::array<import_pair, N> res{};

        for(::std::size_t i{}; i < N; ++i)
        {
            auto const& ipp{ip[1]};
            if(ipp.str.empty() || ipp.func_p == nullptr) { ::fast_io::fast_terminate(); }
            res.index_unchecked(i) = ip[i];
        }

        return res;
    }

    struct hash_table_size
    {
        ::std::size_t size{};
        ::std::size_t extra_size{};
    };

    template <::std::size_t N>
    inline
#if __cpp_consteval >= 201811L
        consteval
#else
        constexpr
#endif
        hash_table_size
        generate_hash_table_size(::fast_io::array<import_pair, N> const& ip) noexcept
    {
        constexpr auto sizet_d10{static_cast<::std::size_t>(::std::numeric_limits<::std::size_t>::digits10)};

        ::fast_io::crc32c_context crc32c{};
        for(auto i{hash_size_base}; i < sizet_d10; ++i)
        {
            ::std::size_t const hash_size{static_cast<::std::size_t>(1u) << i};
            bool c{};
            ::std::size_t extra_size{};
            ::std::size_t* const hash_size_array{new ::std::size_t[hash_size]{}};
            for(auto const& j: ip)
            {
                ::std::size_t const j_str_size{j.str.size()};
                ::std::byte* const ptr{new ::std::byte[j_str_size]{}};
                for(::std::size_t k{}; k < j_str_size; ++k) { ptr[k] = static_cast<::std::byte>(j.str.index_unchecked(k)); }
                crc32c.reset();
                crc32c.update(ptr, ptr + j_str_size);
                delete[] ptr;
                auto const val{crc32c.digest_value() % hash_size};
                ++hash_size_array[val];
                if(hash_size_array[val] == 2) { ++extra_size; }
                if(hash_size_array[val] > max_conflict_size) { c = true; }
            }

            delete[] hash_size_array;
            if(!c) { return {hash_size, extra_size}; }
        }
        ::fast_io::fast_terminate();  // error
        return {};
    }

    struct conflict_table
    {
        ::fast_io::array<import_pair, max_conflict_size> ctmem{};
    };

    template <::std::size_t hash_table_size, ::std::size_t conflict_size>
    struct import_hash_table
    {
        static_assert(hash_table_size > 1);

        ::fast_io::array<import_pair, hash_table_size> ht{};
        ::std::conditional_t<static_cast<bool>(conflict_size), ::fast_io::array<conflict_table, conflict_size>, ::std::in_place_t> ct{};
    };

    template <::std::size_t hash_table_size, ::std::size_t conflict_size, ::std::size_t N>
    inline
#if __cpp_consteval >= 201811L
        consteval
#else
        constexpr
#endif
        auto
        generate_hash_table(::fast_io::array<import_pair, N> const& ip) noexcept
    {
        import_hash_table<hash_table_size, conflict_size> res{};

        ::fast_io::crc32c_context crc32c{};
        ::std::size_t conflictplace{1u};

        for(auto const& j: ip)
        {
            ::std::size_t const j_str_size{j.str.size()};
            ::std::byte* const ptr{new ::std::byte[j_str_size]{}};
            for(::std::size_t k{}; k < j_str_size; ++k) { ptr[k] = static_cast<::std::byte>(j.str.index_unchecked(k)); }
            crc32c.reset();
            crc32c.update(ptr, ptr + j_str_size);
            delete[] ptr;
            auto const val{crc32c.digest_value() % hash_table_size};
            if constexpr(conflict_size)
            {
                if(res.ht.index_unchecked(val).func_p == nullptr)
                {
                    if(!res.ht.index_unchecked(val).str.empty())
                    {
                        for(auto& i: res.ct.index_unchecked(res.ht.index_unchecked(val).str.size() - 1).ctmem)
                        {
                            if(i.func_p == nullptr)
                            {
                                i.func_p = j.func_p;
                                i.str = j.str;
                                break;
                            }
                        }
                    }
                    else
                    {
                        res.ht.index_unchecked(val).func_p = j.func_p;
                        res.ht.index_unchecked(val).str = j.str;
                    }
                }
                else
                {
                    res.ct.index_unchecked(conflictplace - 1).ctmem.front_unchecked().func_p = res.ht.index_unchecked(val).func_p;
                    res.ct.index_unchecked(conflictplace - 1).ctmem.front_unchecked().str = res.ht.index_unchecked(val).str;
                    res.ht.index_unchecked(val).func_p = nullptr;
                    res.ht.index_unchecked(val).str.ptr = nullptr;
                    res.ht.index_unchecked(val).str.n = conflictplace;
                    res.ct.index_unchecked(conflictplace - 1).ctmem.index_unchecked(1).func_p = j.func_p;
                    res.ct.index_unchecked(conflictplace - 1).ctmem.index_unchecked(1).str = j.str;
                    ++conflictplace;
                }
            }
            else
            {
                res.ht.index_unchecked(val).func_p = j.func_p;
                res.ht.index_unchecked(val).str = j.str;
            }
        }
        return res;
    }

    template <::std::size_t hash_table_size, ::std::size_t conflict_size>
    inline constexpr ::uwvm::vm::unchecked_interpreter::int_import_func_p 
        find_from_hash_table(import_hash_table<hash_table_size, conflict_size> const& ht, ::fast_io::u8string_view str) noexcept
    {
        ::fast_io::crc32c_context crc32c{};

#if __cpp_if_consteval >= 202106L
        if consteval
#else
        if(__builtin_is_constant_evaluated())
#endif
        {
            auto const str_size{str.size()};
            ::std::byte* const ptr{new ::std::byte[str_size]{}};
            for(::std::size_t k{}; k < str_size; ++k) { ptr[k] = static_cast<::std::byte>(str.index_unchecked(k)); }
            crc32c.update(ptr, ptr + str_size);
            delete[] ptr;
        }
        else
        {
            auto const i{reinterpret_cast<::std::byte const*>(str.data())};
            crc32c.update(i, i + str.size());
        }
        auto const val{crc32c.digest_value() % hash_table_size};
        auto const htval{ht.ht.index_unchecked(val)};
        if constexpr(conflict_size)
        {
            if(htval.func_p == nullptr)
            {
                if(!htval.str.empty())
                {
                    auto const& ct{ht.ct.index_unchecked(htval.str.size() - 1).ctmem};
                    for(::std::size_t i{}; i < max_conflict_size; ++i)
                    {
                        if(ct.index_unchecked(i).str == str) { return ct.index_unchecked(i).func_p; }
                        else if(ct.index_unchecked(i).func_p == nullptr) [[unlikely]] { return nullptr; }
                    }
                    return nullptr;
                }
                else [[unlikely]] { return nullptr; }
            }
            else
            {
                if(str == htval.str) { return htval.func_p; }
                else [[unlikely]] { return nullptr; }
            }
        }
        else
        {
            if(htval.func_p != nullptr)
            {
                if(str == htval.str) { return htval.func_p; }
                else [[unlikely]] { return nullptr; }
            }
            else [[unlikely]] { return nullptr; }
        }
    }
}  // namespace uwvm::vm::unchecked_interpreter
