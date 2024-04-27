#pragma once

#include <type_traits>
#include <concepts>
#include <compare>
#include <algorithm>

#include <fast_io.h>
#include <fast_io_crypto.h>
#include <fast_io_dsal/array.h>
#include <fast_io_dsal/vector.h>
#include <fast_io_dsal/string_view.h>
#include <fast_io_dsal/cstring_view.h>

namespace uwvm
{
    namespace cmdline
    {

        enum class parameter_parsing_results_type : ::std::uint_fast8_t
        {
            arg = 0,
            occupied_arg,
            parameter,
            invalid_parameter,
            duplicate_parameter,
            dir,
#if 0
            file
#endif
        };

        struct parameter;

        struct parameter_parsing_results
        {
            ::fast_io::cstring_view str{};
            parameter const* para{};
            parameter_parsing_results_type type{};
        };

        enum class parameter_return_type : ::std::uint_fast8_t
        {
            def = 0,
            return_m1_imme,
            return_imme,
            return_soon,
            err_imme,
            err_soon
        };

        using kns_str_scatter_t = ::fast_io::basic_io_scatter_t<::fast_io::string_view>;

        struct parameter
        {
            ::fast_io::string_view const name{};
            ::fast_io::u8string_view const describe{};
            kns_str_scatter_t alias{};
            parameter_return_type (*callback)(parameter_parsing_results* para, ::fast_io::vector<parameter_parsing_results>& vec) noexcept {};
            bool* is_exist{};
        };

        template <::std::size_t N>
        inline consteval auto parameter_sort(parameter const* const (&punsort)[N]) noexcept
        {
            ::fast_io::array<parameter const*, N> res{};
            for(::std::size_t i{}; i < N; ++i) { res.index_unchecked(i) = punsort[i]; }
            ::std::ranges::sort(res,
                                [](parameter const* const a, parameter const* const b) noexcept -> bool
                                {
#if __cpp_lib_three_way_comparison >= 201907L
                                    return a->name < b->name;
#else
                                    return ::std::lexicographical_compare(a->name.cbegin(), a->name.cend(), b->name.cbegin(), b->name.cend());
#endif
                                });
            return res;
        }

        template <::std::size_t N>
        inline constexpr void parameter_clean(::fast_io::array<parameter const*, N>& punsort) noexcept
        {
            for(auto const i: punsort) { i->callback = nullptr; }
        }

        struct all_parameter
        {
            ::fast_io::string_view str{};
            parameter const* para{};
        };

        template <::std::size_t N>
        inline consteval ::std::size_t calculate_all_parameters_size(::fast_io::array<parameter const*, N> const& punsort) noexcept
        {
            ::std::size_t res{};
            for(::std::size_t i{}; i < N; ++i)
            {
                ++res;
                res += punsort.index_unchecked(i)->alias.len;
            }
            return res;
        }

        template <::std::size_t Nres, ::std::size_t N>
        inline consteval auto expand_all_parameters_and_check(::fast_io::array<parameter const*, N> const& punsort) noexcept
        {
            ::fast_io::array<all_parameter, Nres> res{};
            ::std::size_t res_pos{};
            for(::std::size_t i{}; i < N; ++i)
            {
#if 0
                if (punsort.index_unchecked(i)->is_exist == nullptr)
                    ::fast_io::fast_terminate();
#endif
                res.index_unchecked(res_pos++) = {punsort.index_unchecked(i)->name, punsort.index_unchecked(i)};
                for(::std::size_t j{}; j < punsort.index_unchecked(i)->alias.len; ++j)
                {
                    res.index_unchecked(res_pos++) = {punsort.index_unchecked(i)->alias.base[j], punsort.index_unchecked(i)};
                }
            }
            ::std::ranges::sort(res,
                                [](all_parameter const& a, all_parameter const& b) noexcept -> bool
                                {
#if __cpp_lib_three_way_comparison >= 201907L
                                    return a.str < b.str;
#else
                                    return ::std::lexicographical_compare(a.str.cbegin(), a.str.cend(), b.str.cbegin(), b.str.cend());
#endif
                                });
            ::fast_io::string_view check{};  // Empty strings will be sorted and placed first.
            for(auto const& i: res)
            {
                if(i.str == check || i.str.front_unchecked() != '-')
                {
                    ::fast_io::fast_terminate();  // The first character of the parameter must be '-'
                }
                else
                {
                    if(i.str.size() == 1)
                    {
                        ::fast_io::fast_terminate();  // "-" is invalid
                    }
                    check = i.str;
                }
            }
            return res;
        }

        template <::std::size_t N>
        inline consteval ::std::size_t calculate_max_para_size(::fast_io::array<all_parameter, N> const& punsort) noexcept
        {
            ::std::size_t max_size{};
            for(::std::size_t i{}; i < N; ++i) { max_size = ::std::max(max_size, punsort.index_unchecked(i).str.size()); }
            return max_size;
        }

        inline constexpr ::std::size_t hash_size_base{4u};  // 2 ^ hash_size_base
        inline constexpr ::std::size_t max_conflict_size{8u};

        struct calculate_hash_table_size_res
        {
            ::std::size_t hash_table_size{};
            ::std::size_t extra_size{};
        };

        template <::std::size_t N>
        inline consteval calculate_hash_table_size_res calculate_hash_table_size(::fast_io::array<all_parameter, N> const& ord) noexcept
        {
            constexpr auto sizet_d10{static_cast<::std::size_t>(::std::numeric_limits<::std::size_t>::digits10)};

            ::fast_io::crc32c_context crc32c{};
            for(auto i{hash_size_base}; i < sizet_d10; ++i)
            {
                ::std::size_t hash_size{static_cast<::std::size_t>(1u) << i};
                bool c{};
                ::std::size_t extra_size{};
                ::std::size_t* const hash_size_array{new ::std::size_t[hash_size]{}};
                for(auto const& j: ord)
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
        }

        struct ht_para_cpos
        {
            ::fast_io::string_view str{};
            parameter const* para{};
        };

#if 0
struct ct_para_str {
    ::fast_io::string_view str{};
    parameter const* para{};
};
#else
        using ct_para_str = all_parameter;
#endif

        struct conflict_table
        {
            ::fast_io::array<ct_para_str, max_conflict_size> ctmem{};
        };

        template <::std::size_t hash_table_size, ::std::size_t conflict_size>
        struct parameters_hash_table
        {
            static_assert(hash_table_size > 1);

            ::fast_io::array<ht_para_cpos, hash_table_size> ht{};
            ::std::conditional_t<static_cast<bool>(conflict_size), ::fast_io::array<conflict_table, conflict_size>, ::std::in_place_t> ct{};
        };

        template <::std::size_t hash_table_size, ::std::size_t conflict_size, ::std::size_t N>
        inline consteval auto generate_hash_table(::fast_io::array<all_parameter, N> const& ord) noexcept
        {
            parameters_hash_table<hash_table_size, conflict_size> res{};

            ::fast_io::crc32c_context crc32c{};
            ::std::size_t conflictplace{1u};

            for(auto const& j: ord)
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
                    if(res.ht.index_unchecked(val).para == nullptr)
                    {
                        if(!res.ht.index_unchecked(val).str.empty())
                        {
                            for(auto& i: res.ct.index_unchecked(res.ht.index_unchecked(val).str.size() - 1).ctmem)
                            {
                                if(i.para == nullptr)
                                {
                                    i.para = j.para;
                                    i.str = j.str;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            res.ht.index_unchecked(val).para = j.para;
                            res.ht.index_unchecked(val).str = j.str;
                        }
                    }
                    else
                    {
                        res.ct.index_unchecked(conflictplace - 1).ctmem.front_unchecked().para = res.ht.index_unchecked(val).para;
                        res.ct.index_unchecked(conflictplace - 1).ctmem.front_unchecked().str = res.ht.index_unchecked(val).str;
                        res.ht.index_unchecked(val).para = nullptr;
                        res.ht.index_unchecked(val).str.ptr = nullptr;
                        res.ht.index_unchecked(val).str.n = conflictplace;
                        res.ct.index_unchecked(conflictplace - 1).ctmem.index_unchecked(1).para = j.para;
                        res.ct.index_unchecked(conflictplace - 1).ctmem.index_unchecked(1).str = j.str;
                        ++conflictplace;
                    }
                }
                else
                {
                    res.ht.index_unchecked(val).para = j.para;
                    res.ht.index_unchecked(val).str = j.str;
                }
            }
            return res;
        }

        template <::std::size_t hash_table_size, ::std::size_t conflict_size>
        inline constexpr parameter const* find_from_hash_table(parameters_hash_table<hash_table_size, conflict_size> const& ht,
                                                               ::fast_io::string_view str) noexcept
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
                if(htval.para == nullptr)
                {
                    if(!htval.str.empty())
                    {
                        auto const& ct{ht.ct.index_unchecked(htval.str.size() - 1).ctmem};
                        for(::std::size_t i{}; i < max_conflict_size; ++i)
                        {
                            if(ct.index_unchecked(i).str == str) { return ct.index_unchecked(i).para; }
                            else if(ct.index_unchecked(i).para == nullptr) [[unlikely]] { return nullptr; }
                        }
                        return nullptr;
                    }
                    else [[unlikely]] { return nullptr; }
                }
                else
                {
                    if(str == htval.str) { return htval.para; }
                    else [[unlikely]] { return nullptr; }
                }
            }
            else
            {
                if(htval.para != nullptr)
                {
                    if(str == htval.str) { return htval.para; }
                    else [[unlikely]] { return nullptr; }
                }
                else [[unlikely]] { return nullptr; }
            }
        }

    }  // namespace cmdline
}  // namespace uwvm
