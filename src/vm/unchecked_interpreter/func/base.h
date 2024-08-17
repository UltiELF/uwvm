#pragma once

#include "../stack_t.h"

namespace uwvm::vm::unchecked_interpreter::func
{
    namespace details
    {
#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline void
            pop(::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr) noexcept
        {
            --stack_curr;
        }

#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline ::uwvm::vm::unchecked_interpreter::stack_t
            pop_get_val(::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr) noexcept
        {
            --stack_curr;
            return *stack_curr;
        }

#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline void
            push(::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr, ::uwvm::vm::unchecked_interpreter::stack_t val) noexcept
        {
            *stack_curr = val;
            ++stack_curr;
        }

#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline ::uwvm::vm::unchecked_interpreter::stack_t&
            push_get_valref(::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr, ::uwvm::vm::unchecked_interpreter::stack_t val) noexcept
        {
            *stack_curr = val;
            auto const ret{stack_curr};
            ++stack_curr;
            return *ret;
        }

#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        inline ::uwvm::vm::unchecked_interpreter::stack_t&
            top(::uwvm::vm::unchecked_interpreter::stack_t*& stack_curr) noexcept
        {
            return *(stack_curr - 1);
        }

    }  // namespace details
}  // namespace uwvm::vm::unchecked_interpreter::func
