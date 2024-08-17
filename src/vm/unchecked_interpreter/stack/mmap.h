#pragma once
#include <bit>
#include <sys/mman.h>
#include <sys/resource.h>
#include <fast_io.h>
#include <io_device.h>
#include <instrinsic.h>
#include "../stack_t.h"

namespace uwvm::vm::unchecked_interpreter::stack
{
    struct stack_t
    {
        inline static ::std::size_t default_stack_size{static_cast<::std::size_t>(1024)};

        using Alloc = ::fast_io::native_typed_thread_local_allocator<::uwvm::vm::unchecked_interpreter::stack_t>;

        ::uwvm::vm::unchecked_interpreter::stack_t* storage{};
        ::std::byte* memory_begin{};
        ::std::size_t total_pages_bytes{};

        stack_t(stack_t const&) = delete;
        stack_t& operator= (stack_t const&) = delete;

        stack_t() noexcept { init(); };

        void init() noexcept
        {
            auto const stack_size{default_stack_size * sizeof(::uwvm::vm::unchecked_interpreter::stack_t)};
            total_pages_bytes = stack_size + static_cast<::std::size_t>(2 * 4096);

            memory_begin = ::fast_io::details::sys_mmap(nullptr, total_pages_bytes, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

            ::std::byte* stroage_begin{memory_begin + 4096};
            using stack_t_may_alias_ptr
#if __has_cpp_attribute(__gnu__::__may_alias__)
                [[__gnu__::__may_alias__]]
#endif
                = ::uwvm::vm::unchecked_interpreter::stack_t*;

            storage = reinterpret_cast<stack_t_may_alias_ptr>(stroage_begin);

            ::fast_io::details::sys_mprotect(stroage_begin, stack_size, PROT_READ | PROT_WRITE);
        }

        void clear() noexcept
        {
            if(memory_begin) [[likely]]
            {
                if(::fast_io::details::sys_munmap(memory_begin, total_pages_bytes)) [[unlikely]] { ::fast_io::fast_terminate(); }
                storage = nullptr;
                memory_begin = nullptr;
                total_pages_bytes = 0;
            }
        }

        ~stack_t() { clear(); }
    };

}  // namespace uwvm::vm::unchecked_interpreter::stack

namespace fast_io::freestanding
{
    struct is_trivially_relocatable<::uwvm::vm::unchecked_interpreter::stack::stack_t>
    {
        inline static constexpr bool value = true;
    };

    struct is_zero_default_constructible<::uwvm::vm::unchecked_interpreter::stack::stack_t>
    {
        inline static constexpr bool value = true;
    };

}  // namespace fast_io::freestanding
