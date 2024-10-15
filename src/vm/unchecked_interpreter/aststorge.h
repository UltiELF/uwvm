#pragma once
#include <deque>
#include <fast_io_dsal/vector.h>
#if 0
#include <fast_io_dsal/deque.h>
#endif
#include "ast.h"

namespace uwvm::vm::unchecked_interpreter
{
    struct ast_storage_t
    {
        ::fast_io::vector<::uwvm::vm::unchecked_interpreter::ast> asts{};
        // Inserting elements at the beginning and end of the deque container will not render any iterators invalid
        ::std::deque<::fast_io::vector<operator_t>> ext{};
    };

    inline ast_storage_t stroage{};
}

namespace fast_io::freestanding
{
    template <>
    struct is_trivially_relocatable<::uwvm::vm::unchecked_interpreter::ast_storage_t>
    {
        inline static constexpr bool value = true;
    };

    template <>
    struct is_zero_default_constructible<::uwvm::vm::unchecked_interpreter::ast_storage_t>
    {
        inline static constexpr bool value = true;
    };
}  // namespace fast_io::freestanding
