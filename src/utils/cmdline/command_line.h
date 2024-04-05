/**************
 * UWVM *
 *************/

#pragma once

#include <type_traits>
#include <concepts>

#include <fast_io.h>
#include <fast_io_core.h>

namespace uwvm
{
    namespace cmdline
    {
        template <::std::size_t Len = 0, ::std::integral char_type>
        constexpr inline ::std::size_t dp(::fast_io::basic_os_c_str_with_known_size<char_type> x,
                                          ::fast_io::basic_os_c_str_with_known_size<char_type> y) noexcept
        {
            ::std::size_t const lena{x.size()};
            ::std::size_t const lenb{y.size()};

            ::std::size_t* d{};
            if constexpr(Len)
            {
                ::std::size_t temp[Len]{};
                d = temp;
            }
            else
            {
#if __cpp_if_consteval >= 202106L
                if consteval
#else
                if(__builtin_is_constant_evaluated())
#endif
                {
                    d = new ::std::size_t[lenb + 1];
                }
                else { d = ::fast_io::native_typed_global_allocator<::std::size_t>::allocate(lenb + 1); }
            }

            for(::std::size_t j{}; j <= lenb; j++) { d[j] = j; }

            for(::std::size_t i{1}; i <= lena; i++)
            {
                ::std::size_t old{i - 1};
                d[0] = i;
                for(::std::size_t j{1}; j <= lenb; j++)
                {
                    ::std::size_t temp{d[j]};
                    if(x.ptr[i - 1] == y.ptr[j - 1]) { d[j] = old; }
                    else
                    {
                        size_t min = d[j] + 1;
                        if(d[j - 1] + 1 < min) { min = d[j - 1] + 1; }
                        if(old + 1 < min) { min = old + 1; }
                        d[j] = min;
                    }
                    old = temp;
                }
            }

            size_t const ret{d[lenb]};

            if constexpr(!Len)
            {
#if __cpp_if_consteval >= 202106L
                if consteval
#else
                if(__builtin_is_constant_evaluated())
#endif
                {
                    delete[] d;
                }
                else
                {
                    if constexpr(::fast_io::details::has_deallocate_n_impl<::fast_io::native_global_allocator>)
                    {
                        ::fast_io::native_global_allocator::deallocate_n(d, lenb + 1);
                    }
                    else { ::fast_io::native_global_allocator::deallocate(d); }
                }
            }

            return ret;
        }
    }  // namespace cmdline
}  // namespace uwvm
