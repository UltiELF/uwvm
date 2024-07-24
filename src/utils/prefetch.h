#pragma once
#if !__has_builtin(__builtin_prefetch)
    #include <intrin.h>
#endif

namespace uwvm
{
    template <bool write = false, int level = 3>
#if __has_cpp_attribute(__gnu__::__artificial__)
    [[__gnu__::__artificial__]]
#endif
#if __has_cpp_attribute(__gnu__::__always_inline__)
    [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
    [[msvc::forceinline]]
#endif
    inline void
        prefetch(void const* add) noexcept
    {
#if __has_builtin(__builtin_prefetch)
        __builtin_prefetch(add, static_cast<int>(write), level);
#else
        ::_mm_prefetch(reinterpret_cast<char const*>(add), static_cast<int>(write) << 2 | level);
#endif
    }

}  // namespace uwvm
