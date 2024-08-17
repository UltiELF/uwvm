#pragma once
#include <prefetch.h>
#include "ast.h"
#include "aststorge.h"

namespace uwvm::vm::unchecked_interpreter
{

    inline void run_ast(ast const& a, ::uwvm::vm::unchecked_interpreter::stack_t*& stack) noexcept
    {
        if(!a.operators.empty()) [[likely]]
        {
            auto const local_bytes_size{a.local_size * sizeof(::uwvm::vm::unchecked_interpreter::stack_t)};

#if __has_builtin(__builtin_alloca)
            auto local_storage{static_cast<::uwvm::vm::unchecked_interpreter::stack_t*>(__builtin_alloca(local_bytes_size))};
#elif defined(_WIN32) && !defined(__WINE__) && !defined(__BIONIC__) && !defined(__CYGWIN__)
            auto local_storage{static_cast<::uwvm::vm::unchecked_interpreter::stack_t*>(_alloca(local_bytes_size))};
#else
            auto local_storage{static_cast<::uwvm::vm::unchecked_interpreter::stack_t*>(alloca(local_bytes_size))};
#endif

            ::fast_io::freestanding::bytes_clear_n(reinterpret_cast<::std::byte*>(local_storage), local_bytes_size);

            auto const& local_func_type{*a.ft};
            auto const& func_type{*local_func_type.func_type};
            auto const func_type_para_size{static_cast<::std::size_t>(func_type.parameter_end - func_type.parameter_begin)};

            auto const stack_cend{stack};

            auto local_curr_temp{local_storage};

            ::uwvm::prefetch(local_storage);
            for(auto i{stack_cend - func_type_para_size}; i != stack_cend; ++i)
            {
                *local_curr_temp = *i;
                ++local_curr_temp;
            }

            stack -= func_type_para_size;
            ::uwvm::prefetch(stack);

            auto const curr_op{a.operators.begin()};
            curr_op->int_func(curr_op->code_begin, curr_op, local_storage, stack);
        }
    }
}  // namespace uwvm::vm::unchecked_interpreter
