#pragma once
#include "ast.h"

namespace uwvm::vm::interpreter
{
    inline constexpr ::uwvm::vm::interpreter::ast generate_ast(::uwvm::wasm::func_body const& fb) noexcept 
    { 
        auto const local_count{fb.local_count};
        return {};
    }
}  // namespace uwvm::vm::interpreter
