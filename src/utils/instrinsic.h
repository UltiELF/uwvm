#pragma once
#include <concepts>
#include <limits>
#include <bit>

namespace uwvm::instrinsic
{
    template <::std::integral T>
    inline constexpr T floor_log2(T value) noexcept
    {
        constexpr auto dgt{::std::numeric_limits<T>::digits - 1};
        return value <= 1 ? 0 : dgt - ::std::countl_zero(value);
    }

}  // namespace uwvm::instrinsic
