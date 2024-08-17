#pragma once
#include <fast_io.h>
#include <io_device.h>
#include "../ast.h"

namespace uwvm::vm::interpreter::stack
{
    struct stack_t
    {
        inline static ::std::size_t default_stack_size{static_cast<::std::size_t>(1024)};

        using Alloc = ::fast_io::native_typed_thread_local_allocator<::uwvm::vm::interpreter::stack_t>;

        ::uwvm::vm::interpreter::stack_t* storage{};

        stack_t(stack_t const&) = delete;
        stack_t& operator= (stack_t const&) = delete;

        stack_t() noexcept { init(); };

        void init() noexcept { storage = Alloc::allocate_zero(default_stack_size); }

        void clear() noexcept
        {
            if(storage) [[likely]]
            {
                Alloc::deallocate_n(storage, default_stack_size);
                storage = nullptr;
            }
        }

        ~stack_t() { clear(); }

#if __has_cpp_attribute(__gnu__::__always_inline__)
        [[__gnu__::__always_inline__]]
#elif __has_cpp_attribute(msvc::forceinline)
        [[msvc::forceinline]]
#endif
        static constexpr ::uwvm::vm::interpreter::stack_t*
            checkptr(::uwvm::vm::interpreter::stack_t* ptr, const stack_t* bst) noexcept
        {
            if(ptr < bst.storage || ptr - bst.storage >= default_stack_size) [[unlikely]] { ::fast_io::fast_terminate(); }
            return ptr;
        }
    };

}  // namespace uwvm::vm::interpreter::stack

namespace fast_io::freestanding
{
    struct is_trivially_relocatable<::uwvm::vm::interpreter::stack::stack_t>
    {
        inline static constexpr bool value = true;
    };

    struct is_zero_default_constructible<::uwvm::vm::interpreter::stack::stack_t>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
