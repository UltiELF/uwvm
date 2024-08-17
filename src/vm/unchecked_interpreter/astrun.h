#pragma once
#include <prefetch.h>
#include "ast.h"
#include "aststorge.h"

namespace uwvm::vm::unchecked_interpreter
{

    inline void run_ast(ast const& a, ::uwvm::vm::unchecked_interpreter::stack_t* stack) noexcept
    {

        if(!a.operators.empty()) [[likely]]
        {
#if __has_builtin(__builtin_alloca)
            auto local_storage{
                static_cast<::uwvm::vm::unchecked_interpreter::stack_t*>(__builtin_alloca(a.local_size * sizeof(::uwvm::vm::unchecked_interpreter::stack_t)))};
#elif defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__) && !defined(__CYGWIN__)
            auto local_storage{
                static_cast<::uwvm::vm::unchecked_interpreter::stack_t*>(_alloca(a.local_size * sizeof(::uwvm::vm::unchecked_interpreter::stack_t)))};
#else
            auto local_storage{
                static_cast<::uwvm::vm::unchecked_interpreter::stack_t*>(alloca(a.local_size * sizeof(::uwvm::vm::unchecked_interpreter::stack_t)))};
#endif

            ::uwvm::prefetch(stack);
            ::uwvm::prefetch(local_storage);

            auto const curr_op{a.operators.begin()};
            curr_op->int_func(curr_op->code_begin, curr_op, local_storage, stack);
        }
    }
}  // namespace uwvm::vm::unchecked_interpreter
