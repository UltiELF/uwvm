/**************
 * UWVM *
 *************/

#pragma once

#include <type_traits>
#include <concepts>

#include <fast_io.h>
#include <fast_io_core.h>
#include <fast_io_dsal/array.h>

namespace uwvm
{
    namespace cmdline
    {
        template <::std::size_t Len = 0, ::std::integral char_type>
#if __has_cpp_attribute(__gnu__::__const__)
        [[__gnu__::__const__]]
#endif
        inline constexpr ::std::size_t
            dp(char_type const* x, ::std::size_t lena, char_type const* y, ::std::size_t lenb) noexcept
        {
            using Alloc = ::fast_io::native_typed_thread_local_allocator<::std::size_t>;

            ::std::size_t* d{};
            [[maybe_unused]] ::fast_io::array<::std::size_t, Len> storage;

            if constexpr(Len) { d = storage.data(); }
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
                else { d = Alloc::allocate(lenb + 1); }
            }

            for(::std::size_t j{}; j <= lenb; j++) { d[j] = j; }

            for(::std::size_t i{1}; i <= lena; i++)
            {
                ::std::size_t old{i - 1};
                d[0] = i;
                for(::std::size_t j{1}; j <= lenb; j++)
                {
                    ::std::size_t const temp{d[j]};
                    if(x[i - 1] == y[j - 1]) { d[j] = old; }
                    else
                    {
                        ::std::size_t min = d[j] + 1;
                        if(d[j - 1] + 1 < min) { min = d[j - 1] + 1; }
                        if(old + 1 < min) { min = old + 1; }
                        d[j] = min;
                    }
                    old = temp;
                }
            }

            ::std::size_t const ret{d[lenb]};

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
                    if constexpr(::fast_io::details::has_deallocate_n_impl<Alloc>)
                    {
                        Alloc::deallocate_n(d, lenb + 1);
                    }
                    else { Alloc::deallocate(d); }
                }
            }

            return ret;
        }
    }  // namespace cmdline
}  // namespace uwvm
