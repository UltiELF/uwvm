#pragma once
#include "../../section/custom.h"
#include "../../module.h"

namespace uwvm::wasm::custom
{
    inline constexpr bool scan_name_custom_section(::uwvm::wasm::section::custom_section cs) noexcept 
    {
        return false;

    }

}  // namespace uwvm::wasm::custom
