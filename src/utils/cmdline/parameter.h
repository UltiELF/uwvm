#pragma once

#include <type_traits>
#include <concepts>
#include <compare>
#include <algorithm>

#include <fast_io.h>
#include <fast_io_dsal/vector.h>
#include <fast_io_crypto.h>

namespace fast_io
{
    template <::std::integral char_type>
    constexpr inline bool operator== (basic_os_c_str_with_known_size<char_type> a, basic_os_c_str_with_known_size<char_type> b) noexcept
    {
        if(a.n != b.n) { return false; }
        else
        {
            for(const auto a_last{a.ptr + a.n}; a.ptr != a_last;)
            {
                if(*a.ptr != *b.ptr) { return false; }
                ++a.ptr;
                ++b.ptr;
            }
            return true;
        }
    }

    template <::std::integral char_type>
    constexpr inline ::std::strong_ordering operator<=> (basic_os_c_str_with_known_size<char_type> a, basic_os_c_str_with_known_size<char_type> b) noexcept
    {
        const auto a_last{a.ptr + a.n};
        const auto b_last{b.ptr + b.n};
        for(; (a.ptr != a_last) && (b.ptr != b_last);)
        {
            if(auto i{*a.ptr <=> *b.ptr}; i != ::std::strong_ordering::equal) { return i; }
            ++a.ptr;
            ++b.ptr;
        }
        return a.n <=> b.n;
    }
}  // namespace fast_io::manipulators

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
            ::fast_io::basic_os_c_str_with_known_size<char> str{};
            const parameter* para{};
            parameter_parsing_results_type type{};
        };

        enum class parameter_return_type : ::std::uint_fast8_t
        {
            def = 0,
            return_imme,
            return_soon,
            err_imme,
            err_soon
        };

        using kns_str_scatter_t = ::fast_io::basic_io_scatter_t<::fast_io::basic_os_c_str_with_known_size<char>>;

        struct parameter
        {
            const ::fast_io::basic_os_c_str_with_known_size<char> name{};
            const ::fast_io::basic_os_c_str_with_known_size<char8_t> describe{};
            kns_str_scatter_t alias{};
            parameter_return_type (*callback)(::std::size_t argc, ::fast_io::vector<parameter_parsing_results>& vec) noexcept {};
            bool* is_exist{};
        };

        template <::std::size_t N>
        inline consteval auto parameter_sort(const parameter* const (&punsort)[N]) noexcept
        {
            ::fast_io::freestanding::array<const parameter*, N> res{};
            for(::std::size_t i{}; i < N; i++) { res[i] = punsort[i]; }
            ::std::ranges::sort(res, [](const parameter* const a, const parameter* const b) noexcept -> bool { return a->name < b->name; });
            return res;
        }

        template <::std::size_t N>
        constexpr inline void parameter_clean(::fast_io::freestanding::array<const parameter*, N>& punsort) noexcept
        {
            for(auto i: punsort) { i.callback = nullptr; }
        }

        struct all_parameter
        {
            ::fast_io::basic_os_c_str_with_known_size<char> str{};
            const parameter* para{};
        };

        template <::std::size_t N>
        inline consteval ::std::size_t calculate_all_parameters_size(const ::fast_io::freestanding::array<const parameter*, N>& punsort) noexcept
        {
            ::std::size_t res{};
            for(::std::size_t i{}; i < N; i++)
            {
                res++;
                res += punsort[i]->alias.len;
            }
            return res;
        }

        template <::std::size_t Nres, ::std::size_t N>
        inline consteval auto expand_all_parameters_and_check(const ::fast_io::freestanding::array<const parameter*, N>& punsort) noexcept
        {
            ::fast_io::freestanding::array<all_parameter, Nres> res{};
            ::std::size_t res_pos{};
            for(::std::size_t i{}; i < N; i++)
            {
#if 0
		if (punsort[i]->is_exist == nullptr)
			::fast_io::fast_terminate();

#endif
                res[res_pos++] = {punsort[i]->name, punsort[i]};
                for(::std::size_t j{}; j < punsort[i]->alias.len; j++) { res[res_pos++] = {punsort[i]->alias.base[j], punsort[i]}; }
            }
            ::std::ranges::sort(res, [](const all_parameter& a, const all_parameter& b) noexcept -> bool { return a.str < b.str; });
            ::fast_io::basic_os_c_str_with_known_size<char> check{};  // Empty strings will be sorted and placed first.
            for(auto& i: res)
            {
                if(i.str == check || i.str.ptr[0] != '-')
                {
                    ::fast_io::fast_terminate();  // The first character of the parameter must be '-'
                }
                else
                {
                    if(i.str.n == 1)
                    {
                        ::fast_io::fast_terminate();  // "-" is invalid
                    }
                    check = i.str;
                }
            }
            return res;
        }

        template <::std::size_t N>
        inline consteval ::std::size_t calculate_max_para_size(const ::fast_io::freestanding::array<all_parameter, N>& punsort) noexcept
        {
            ::std::size_t max_size{};
            for(::std::size_t i{}; i < N; i++) { max_size = ::std::max(max_size, punsort[i].str.size()); }
            return max_size;
        }

        constexpr inline ::std::size_t hash_size_base{4u};
        constexpr inline ::std::size_t max_conflict_size{8u};

        struct calculate_hash_table_size_res
        {
            ::std::size_t hash_table_size{};
            ::std::size_t extra_size{};
        };

        template <::std::size_t N>
        inline consteval calculate_hash_table_size_res calculate_hash_table_size(const ::fast_io::freestanding::array<all_parameter, N>& ord) noexcept
        {
            constexpr auto sizet_d10{::std::numeric_limits<::std::size_t>::digits10};

            ::fast_io::crc32c_context crc32c{};
            for(int i{hash_size_base}; i < sizet_d10; i++)
            {
                ::std::size_t hash_size{static_cast<::std::size_t>(1u) << i};
                bool c{};
                ::std::size_t extra_size{};
                ::std::size_t* const hash_size_array{new ::std::size_t[hash_size]{}};
                for(auto& j: ord)
                {
                    const ::std::size_t j_str_size{j.str.n};
                    ::std::byte* const ptr{new ::std::byte[j_str_size]{}};
                    for(::std::size_t k{}; k < j_str_size; k++) { ptr[k] = static_cast<::std::byte>(j.str.ptr[k]); }
                    crc32c.reset();
                    crc32c.update(ptr, ptr + j_str_size);
                    delete[] ptr;
                    const auto val{crc32c.digest_value() % hash_size};
                    hash_size_array[val]++;
                    if(hash_size_array[val] == 2) { extra_size++; }
                    if(hash_size_array[val] > max_conflict_size) { c = true; }
                }

                delete[] hash_size_array;
                if(!c) { return {hash_size, extra_size}; }
            }
            ::fast_io::fast_terminate();  // error
        }

        struct ht_para_cpos
        {
            ::fast_io::basic_os_c_str_with_known_size<char> str{};
            const parameter* para{};
        };

#if 0
struct ct_para_str {
	::fast_io::basic_os_c_str_with_known_size<char> str{};
	parameter const* para{};
};
#else
        using ct_para_str = all_parameter;
#endif  // 0

        struct conflict_table
        {
            ::fast_io::freestanding::array<ct_para_str, max_conflict_size> ctmem{};
        };

        template <::std::size_t hash_table_size, ::std::size_t conflict_size>
        struct parameters_hash_table
        {
            static_assert(hash_table_size > 1);

            ::fast_io::freestanding::array<ht_para_cpos, hash_table_size> ht{};
            ::std::conditional_t<static_cast<bool>(conflict_size), ::fast_io::freestanding::array<conflict_table, conflict_size>, ::std::in_place_t> ct{};
        };

        template <::std::size_t hash_table_size, ::std::size_t conflict_size, ::std::size_t N>
        inline consteval auto generate_hash_table(const ::fast_io::freestanding::array<all_parameter, N>& ord) noexcept
        {
            parameters_hash_table<hash_table_size, conflict_size> res{};

            ::fast_io::crc32c_context crc32c{};
            ::std::size_t conflictplace{1u};

            for(auto& j: ord)
            {
                const ::std::size_t j_str_size{j.str.n};
                ::std::byte* const ptr{new ::std::byte[j_str_size]{}};
                for(::std::size_t k{}; k < j_str_size; k++) { ptr[k] = static_cast<::std::byte>(j.str.ptr[k]); }
                crc32c.reset();
                crc32c.update(ptr, ptr + j_str_size);
                delete[] ptr;
                const auto val{crc32c.digest_value() % hash_table_size};
                if constexpr(conflict_size)
                {
                    if(res.ht[val].para == nullptr)
                    {
                        if(res.ht[val].str.n != 0)
                        {
                            for(auto& i: res.ct[res.ht[val].str.n - 1].ctmem)
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
                            res.ht[val].para = j.para;
                            res.ht[val].str = j.str;
                        }
                    }
                    else
                    {
                        res.ct[conflictplace - 1].ctmem[0].para = res.ht[val].para;
                        res.ct[conflictplace - 1].ctmem[0].str = res.ht[val].str;
                        res.ht[val].para = nullptr;
                        res.ht[val].str.ptr = nullptr;
                        res.ht[val].str.n = conflictplace;
                        res.ct[conflictplace - 1].ctmem[1].para = j.para;
                        res.ct[conflictplace - 1].ctmem[1].str = j.str;
                        conflictplace++;
                    }
                }
                else
                {
                    res.ht[val].para = j.para;
                    res.ht[val].str = j.str;
                }
            }
            return res;
        }

        template <::std::size_t hash_table_size, ::std::size_t conflict_size>
        constexpr const inline parameter* find_from_hash_table(const parameters_hash_table<hash_table_size, conflict_size>& ht,
                                                               ::fast_io::basic_os_c_str_with_known_size<char> str) noexcept
        {
            ::fast_io::crc32c_context crc32c{};

#if __cpp_if_consteval >= 202106L
            if consteval
#else
            if(__builtin_is_constant_evaluated())
#endif
            {
                const auto str_size{str.size()};
                ::std::byte* const ptr{new ::std::byte[str_size]{}};
                for(::std::size_t k{}; k < str_size; k++) { ptr[k] = static_cast<::std::byte>(str.ptr[k]); }
                crc32c.update(ptr, ptr + str_size);
                delete[] ptr;
            }
            else
            {
                auto i{reinterpret_cast<const ::std::byte*>(str.data())};
                crc32c.update(i, i + str.size());
            }
            const auto val{crc32c.digest_value() % hash_table_size};
            const auto htval{ht.ht[val]};
            if constexpr(conflict_size)
            {
                if(htval.para == nullptr)
                {
                    if(htval.str.n != 0)
                    {
                        auto& ct{ht.ct[htval.str.n - 1].ctmem};
                        for(::std::size_t i{}; i < max_conflict_size; i++)
                        {
                            if(ct[i].str == str) { return ct[i].para; }
                            else if(ct[i].para == nullptr) { return nullptr; }
                        }
                        return nullptr;
                    }
                    else { return nullptr; }
                }
                else
                {
                    if(str == htval.str) { return htval.para; }
                    else { return nullptr; }
                }
            }
            else
            {
                if(htval.para != nullptr)
                {
                    if(str == htval.str) { return htval.para; }
                    else { return nullptr; }
                }
                else { return nullptr; }
            }
        }

    }  // namespace cmdline
}  // namespace uwvm
