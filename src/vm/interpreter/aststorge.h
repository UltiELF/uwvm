#pragma once
#include <fast_io_dsal/vector.h>
#include "ast.h"

namespace uwvm::vm::interpreter
{
    struct ast_storage_t
    {
        ::fast_io::vector<::uwvm::vm::interpreter::ast> asts{};
    };

    inline ast_storage_t stroage{};
}

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::interpreter::ast_storage_t>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::interpreter::ast_storage_t>
    {
        inline static constexpr bool value = true;
    };
}  // namespace fast_io::freestanding
