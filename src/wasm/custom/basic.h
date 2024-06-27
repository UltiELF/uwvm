#pragma once
#include "../../run/features.h"

namespace uwvm::wasm::custom
{
    inline bool check_custom_index(::std::size_t s) noexcept
    {
#if defined(__SIZEOF_SIZE_T__) && __SIZEOF_SIZE_T__ == 8 || 1
        auto const has_enable_memory64_alias{::uwvm::features::enable_memory64};
        constexpr auto u32max{static_cast<::std::size_t>(::std::numeric_limits<::std::uint_least32_t>::max())};
        if(!has_enable_memory64_alias && s > u32max) [[unlikely]] { return false; }
#else
        if constexpr(sizeof(::std::size_t) == 8)
        {
            auto const has_enable_memory64_alias{::uwvm::features::enable_memory64};
            constexpr auto u32max{static_cast<::std::size_t>(::std::numeric_limits<::std::uint_least32_t>::max())};
            if(!has_enable_memory64_alias && s > u32max) [[unlikely]] { return false; }
        }
#endif
        return true;
    }

}  // namespace uwvm::wasm::custom
